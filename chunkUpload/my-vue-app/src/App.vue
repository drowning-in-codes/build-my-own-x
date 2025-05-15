<script setup lang="ts">
import { ref } from "vue";
import SparkMD5 from "spark-md5";
const CHUNK_SIZE = 1024 * 1024; // 1MB
const file = ref<File | null>(null);
const fileName = ref<string | null>(null);
const fileHash = ref<string | null>(null);
const worker = ref<Worker | null>(null);
if(window.Worker){
  console.log("支持worker");
console.log(import.meta.url);
   worker.value = new Worker(new URL('./worker.js', import.meta.url), {
  type: 'module',
})
}
console.log(import.meta.url);
const handleFileChange = async (event: Event) => {
  const target = event.target as HTMLInputElement;
  if (target.files && target.files.length > 0) {
    file.value = target.files[0] as File;
    fileName.value = file.value.name;
    const chunks = calculateChunks(file.value);
    const hash = (await calculateHash(chunks)) as string;
    fileHash.value = hash;
    // 校验hash
    try {
      const { existFile, existChunks } = await verifyHash(hash, fileName.value);
      // if (!existFile) {
      //   // 上传分片
      //   uploadChunks(chunks);
      // }else{
      //   if(chunkIndex){
      //     // 断点续传
      //     const newChunks = chunks.slice(chunkIndex);
      //     uploadChunks(newChunks);
      //   }
      // }
      if (!existFile) {
        // 如果不存在完整文件,上传
        uploadChunks(chunks, existChunks);
      } else {
        alert("文件已存在");
      }
    } catch (error) {
      console.error(error);
    }
  }
};

const verifyHash = async (hash: string, fileName: string) => {
  return fetch("http://localhost:3000/verify", {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
    },
    body: JSON.stringify({
      fileHash: hash,
      fileName: fileName,
    }),
  })
    .then((resp) => resp.json())
    .then((result) => {
      return {
        existFile: !result.shouldUpload,
        existChunks: result?.existChunks,
      };
    })
    .catch((error) => {
      console.error("Error verifying file hash:", error);
      throw new Error(error);
    });
};
const uploadChunks = async (
  chunks: Blob[],
  existChunks: string[] | null | undefined
) => {
  const data = chunks
    .filter((_, index) => !existChunks?.includes(fileHash.value + "-" + index))
    .map((chunk: Blob, index: number) => {
      const formData = new FormData();
      formData.append("fileHash", fileHash.value as string);
      formData.append("chunk", chunk);
      formData.append("size", chunk.size.toString());
      formData.append("chunkIndex", index.toString());
      formData.append("chunkHash", fileHash.value + "-" + index);
      return formData;
    });
  const MAX_REQUEST = 6;
  // 并发请求
  const taskPool: Promise<Response>[] = [];
  let index = 0;
  while (index < data.length) {
    const task = fetch("http://localhost:3000/upload", {
      method: "POST",
      body: data[index],
    });
    taskPool.push(task);

    task
      .then((response: Response) => {
        if (response.ok) {
          console.log("分片上传成功");
          taskPool.splice(taskPool.indexOf(task), 1);
        } else {
          console.error("分片上传失败");
        }
      })
      .catch((error) => {
        // 因为网络问题断点续传

        console.error("Error uploading chunk:", error);
      });
    if (taskPool.length == MAX_REQUEST) {
      Promise.race(taskPool)
        // 只要有一个完成就返回
        .then((result) => {
          if (result.ok) {
            console.log("分片上传成功");
            // 如果上传成功,移除该任务
            taskPool.splice(taskPool.indexOf(task), 1);
          } else {
            console.error("分片上传失败");
          }
        })
        .catch((error) => {
          console.error("Error uploading chunk:", error);
        });
    }
    index++;
  }
  await Promise.all(taskPool); // 等待所有请求完成

  // 发送完毕,发送切片合并请求
  mergeRequest();
};
const mergeRequest = async () => {
  const reqData = {
    fileHash: fileHash.value,
    fileName: fileName.value,
    fileTotalSize: file.value?.size,
    chunkSize: CHUNK_SIZE,
  };
  const response = await fetch("http://localhost:3000/merge", {
    headers: {
      "Content-Type": "application/json",
    },
    method: "POST",
    body: JSON.stringify(reqData),
  });
  if (response.ok) {
    console.log("文件合并成功");
    alert("合并成功");
  } else {
    console.error("文件合并失败");
  }
};
const calculateChunks = (file: File): Blob[] => {
  const size = file.size;
  const chunks: Blob[] = [];
  let start = 0;
  while (start < size) {
    const end = Math.min(start + CHUNK_SIZE, size);
    const chunk = file.slice(start, end);
    chunks.push(chunk);
    start = end;
  }
  return chunks;
};

const calculateHash = async (chunks: Blob[]) => {
  return new Promise((resolve, reject) => {
    const size = chunks.length;
    console.log(`一共有${size}个分片`);
    const targets: Blob[] = [];
    const fileReader = new FileReader();
    chunks.forEach((chunk: Blob, index) => {
      if (index == 0 || index == size - 1) {
        targets.push(chunk);
      } else {
        targets.push(chunk.slice(0, 2));
        targets.push(chunk.slice(CHUNK_SIZE / 2, CHUNK_SIZE / 2 + 2));
        targets.push(chunk.slice(CHUNK_SIZE - 2, CHUNK_SIZE));
      }
    });
    fileReader.readAsArrayBuffer(new Blob(targets));
    const spark = new SparkMD5.ArrayBuffer();
    fileReader.onload = (event: ProgressEvent<FileReader>) => {
      const arrayBuffer = event.target?.result as ArrayBuffer;
      if (worker.value) {
        // 支持worker
        worker.value.onmessage = function (event) {
          resolve(event.data);
        };
        worker.value.postMessage(arrayBuffer);
      } else {
        spark.append(arrayBuffer);
        resolve(spark.end());
      }
    };

    fileReader.onerror = (event: ProgressEvent<FileReader>) => {
      console.error("Error reading file:", event);
      reject(event);
    };
  });
};
</script>

<template>
  <div>
    <label for="file">传输文件</label>
    <input
      type="file"
      title="file"
      id="file"
      ref="file"
      @change="handleFileChange"
    />
  </div>
</template>

<style scoped>
.logo {
  height: 6em;
  padding: 1.5em;
  will-change: filter;
  transition: filter 300ms;
}

.logo:hover {
  filter: drop-shadow(0 0 2em #646cffaa);
}

.logo.vue:hover {
  filter: drop-shadow(0 0 2em #42b883aa);
}
</style>
