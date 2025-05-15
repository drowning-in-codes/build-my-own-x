const express = require("express");
const fs = require("fs");
const path = require("path");
const fse = require("fs-extra");
var multiparty = require("multiparty");
const cors = require("cors");
const { pipeline } = require("stream/promises");
const UPLOAD_DIR = path.resolve(__dirname, "uploads");
const PORT = 3000;

const app = express();
app.use(express.json());
app.use(express.urlencoded({ extended: true }));
app.use(cors());
app.post("/upload", (req, res) => {
  var form = new multiparty.Form();
  form.parse(req, async function (err, fields, files) {
    if (err) {
      res.status(401).json({
        ok: false,
        msg: "上传失败,请重新上传",
      });
      return;
    }
    const {
      fileHash: fileHashArray,
      chunkHash: chunkHashArray,
      chunkIndex: chunkIndexArray,
      size: sizeArray,
    } = fields;
    const chunkPath = path.resolve(UPLOAD_DIR, fileHashArray[0]);
    if (!fs.existsSync(chunkPath)) {
      fs.mkdirSync(chunkPath);
    }
    // 文件chunk
    const chunkFile = files["chunk"];
    if (chunkFile == undefined || chunkFile.length == 0) {
      res.status(401).json({
        ok: false,
        msg: "上传失败,请重新上传",
      });
      return;
    }
    const filePath = chunkFile[0].path;
    try {
      await fse.move(filePath, path.resolve(chunkPath, chunkHashArray[0]));
    } catch (err) {
      console.log(err);
      res.status(401).json({
        ok: false,
        msg: "上传失败,请重新上传",
      });
      return;
    }

    res.status(200).json({
      ok: true,
      msg: "上传成功",
    });
  });
});
const getSuffix = (fileName) => {
  return "." + fileName.split(".").pop();
};

app.post("/verify", (req, res) => {
  const { fileHash, fileName } = req.body;

  // 返回服务器已经上传成功的切片索引
  const chunkDir = path.resolve(UPLOAD_DIR, fileHash);
  if (fs.existsSync(chunkDir)) {
    // 如果存在该目录,查看其中的最大文件索引
    const files = fs.readdirSync(chunkDir);
    // const maxIndex = Math.max(...files.map((fileName) => {
    //   return parseInt(fileName.split("-")[1]);
    // }));
    res.status(200).json({
      ok: false,
      msg: "完整文件不存在",
      shouldUpload:true,
      existChunks: files,
    });
  }


  // 验证是否已经有合并后的完整文件
  const suffix = getSuffix(fileName);
  const filePath = path.resolve(UPLOAD_DIR, fileHash + suffix);
  if (fs.existsSync(filePath)) {
    res.status(200).json({
      ok: true,
      msg: "文件已经存在",
      shouldUpload:false,
    });
  } else {
    res.status(200).json({
      ok: false,
      msg: "完整文件不存在",
      shouldUpload:true,
    });
  }

})
app.post("/merge", async (req, res) => {
  // 合并文件 json请求
  const { fileHash, fileName, fileTotalSize, chunkSize } = req.body;
  const chunkFile = path.resolve(UPLOAD_DIR, fileHash + getSuffix(fileName));
  if (fs.existsSync(chunkFile)) {
    res.status(200).json({
      ok: true,
      msg: "文件已经存在",
    });
  } else {
    const chunkDir = path.resolve(UPLOAD_DIR, fileHash);
    if (!fs.existsSync(chunkDir)) {
      res.status(401).json({
        ok: false,
        msg: "合并失败,请重新上传",
      });
    }
    // 读取目录下文件
    const mergeFilePath = chunkFile;
    const chunkPaths = await fse.readdir(chunkDir);
    if (chunkPaths.length === 0) {
      res.status(401).json({
        ok: false,
        msg: "合并失败,请重新上传",
      });
    }
    chunkPaths.sort((a, b) => {
      return a.split("-")[1] - b.split("-")[1];
    });
    const tasks = chunkPaths.map((chunkPath, index) => {
      return new Promise((resolve, reject) => {
        // 读取文件
        const chunkFilePath = path.resolve(chunkDir, chunkPath);
        // const fileBuffer = fs.readFileSync(chunkFilePath);
        // 写入文件
        const readStream = fse.createReadStream(chunkFilePath);
        const writeStream = fse.createWriteStream(mergeFilePath, {
          start: index * chunkSize,
          end: (index + 1) * chunkSize,
        });
        // writeStream.on("finish", async () => {
        //   console.log(`删除文件${chunkFilePath}`);
        //   await fse.unlink(chunkFilePath);
        // });
        try {
          pipeline(readStream, writeStream).then(() => {
            fse.unlinkSync(chunkFilePath);
            resolve();
          }).catch((err) => {
            console.log(err);
            reject(err);
          });
        } catch (err) {
          console.log(err);
          reject(err);
        }
        FileReader()
        // fs.appendFileSync(mergeFilePath, fileBuffer);
      })
    });
    try {
      await Promise.all(tasks);
    } catch (err) {
      res.status(401).json({
        ok: false,
        msg: "合并失败,请重新上传",
      });
      return;
     
    }
    const files = await fse.readdir(chunkDir);
    const fileCount = files.length;
    console.log(fileCount)
    if (fileCount === 0) {
      await fse.rmdir(chunkDir);
    }
    res.status(200).json({
      ok: true,
      msg: "合并成功",
    });
  }
});

const init = () => {
  if (!fs.existsSync(UPLOAD_DIR)) {
    fs.mkdirSync(UPLOAD_DIR);
  }
};
init();
app.listen(PORT, () => {
  console.log(`Server is running on port ${PORT}`);
});
