// 引入 spark-md5（注意路径）或使用 CDN + importScripts
importScripts('https://cdn.jsdelivr.net/npm/spark-md5@3.0.2/spark-md5.min.js');

onmessage = function (event) {
  const arrayBuffer = event.data;
  const spark = new self.SparkMD5.ArrayBuffer();
  spark.append(arrayBuffer);
  const hash = spark.end();
  postMessage(hash);
};
