const WebSocket = require("ws");
const port = 8080;
const server = new WebSocket.Server({
  port: port,
});
const userIDMap = new Map();
function send(client, message, userID) {
  const data = { message: message };
  if (userID != undefined || userID != null) {
    data["user"] = userID;
  }
  const json = JSON.stringify(data);
  client.send(json);
}
console.log(`Server is running on port ${port}`);
let sockets = [];
server.on("connection", (socket) => {
  console.log("New connection " + `Total connections: ${sockets.length}`);
  sockets.push(socket);

  socket.on("message", (message) => {
    const receivedData = JSON.parse(message);
    console.log(receivedData);
    socket.userId = receivedData.user;
    userIDMap.set(receivedData.user, socket);
    let sendMsg = {
      userNumbers: sockets.length,
      users: Array.from(userIDMap.keys()),
    };
    sockets.forEach((s) => {
      if (s !== socket) {
        // s.send(receivedData);
        send(s, receivedData.message, receivedData.user);
      }
      send(s, sendMsg);
    });
  });
  socket.on("error", () => {
    console.log("Error in connection");
  });
  socket.on("close", () => {
    sockets = sockets.filter((s) => s !== socket);
    userIDMap.delete(socket.userId);
    console.log("Connection closed " + `Total connections: ${sockets.length}`);
  });
});
