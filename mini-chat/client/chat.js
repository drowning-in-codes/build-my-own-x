const messageBox = document.querySelector(".chat-input");
const btn = document.querySelector(".send-btn");
const sideBar = document.querySelector(".sidebar");
const userIdBlock = document.querySelector(".user-id");
const chatContent = document.querySelector(".chat-content");
const marginGap = 20;

function send(client, userID, message) {
  const data = { user: userID, message: message };
  const json = JSON.stringify(data);
  client.send(json);
}
document.addEventListener("DOMContentLoaded", () => {
  const client = new WebSocket("ws://localhost:8080");
  const userID = createUserId();
  userIdBlock.textContent = userID.toString();
  client.addEventListener("open", function (event) {
    console.log("Connected to server");

    send(client, userID, `Hello Everyone!, I'm ${userID}`);
  });
  // Listen for messages
  client.addEventListener("message", function (event) {
    const jsonData = JSON.parse(event.data);
    if ("user" in jsonData) {
      // 消息前加上userid
      const messageElement = document.createElement("div");
      messageElement.classList.add("chat-bubble-other");
      const userIdElement = document.createElement("div");
      userIdElement.textContent = jsonData["user"];
      userIdElement.classList.add("chat-user-id");
      messageElement.appendChild(userIdElement);

      const spanMessage = document.createElement("span");
      spanMessage.textContent = jsonData["message"];
      messageElement.appendChild(spanMessage);
      chatContent.insertAdjacentElement("beforeend", messageElement);
    } else {
      const ele = document.querySelector(".chat-notice");
      if (ele !== null) {
        const noticeNumberDiv = document.querySelector(".chat-notice-number");
        noticeNumberDiv.textContent =
          "Current User Count: " + jsonData["message"].userNumbers;
        const noticeUserDiv = document.querySelector(".chat-notice-user");
        noticeUserDiv.textContent = "Users: ";
        jsonData["message"].users.forEach((user, index) => {
          const userSpan = document.createElement("div");
          userSpan.textContent = `${index + 1}: ` + user;
          noticeUserDiv.appendChild(userSpan);
        });
      } else {
        const noticeElement = document.createElement("div");
        noticeElement.classList.add("chat-notice");
        const noticeNumberDiv = document.createElement("div");
        noticeNumberDiv.classList.add("chat-notice-number");
        noticeNumberDiv.textContent =
          "Current User Count: " + jsonData["message"].userNumbers;
        noticeElement.appendChild(noticeNumberDiv);

        if (jsonData["message"].users.length > 0) {
          const noticeUserDiv = document.createElement("div");
          noticeUserDiv.classList.add("chat-notice-user");
          noticeUserDiv.textContent = "Users: ";
          jsonData["message"].users.forEach((user, index) => {
            const userSpan = document.createElement("div");
            userSpan.textContent = `${index + 1}: ` + user;
            noticeUserDiv.appendChild(userSpan);
          });
          noticeElement.appendChild(noticeUserDiv);
        }
        sideBar.insertAdjacentElement("afterbegin", noticeElement);
      }

      //   noticeElement.textContent = data;
    }
    console.log("Message from server ", event.data);
  });
  messageBox.addEventListener("keyup", (e) => {
    console.log(e);
    if (e.key === "Enter") {
      e.preventDefault();

      sendMessage(client, userID, messageBox);
    }
  });
  btn.addEventListener("click", () => {
    sendMessage(client, userID, messageBox);
  });
});
// 建立websocket连接
function createUserId() {
  const timestamp = Date.now();
  return (
    timestamp.toString(36).substring(5, 12) +
    Math.random().toString().substring(2, 8)
  );
}
function sendMessage(client, userID, messageBox) {
  //   使用websocket发送消息
  const message = messageBox.value;
  if (message.toString().trim() === "") return;
  send(client, userID, message);
  const messageElement = document.createElement("div");
  const messageClass =
    "chat-bubble-self" + `${Date.now().toString().substring(5, 12)}`;

  messageElement.classList.add(messageClass);
  const spanMessage = document.createElement("span");
  spanMessage.textContent = message;
  messageElement.appendChild(spanMessage);
  chatContent.insertAdjacentElement("beforeend", messageElement);
  messageBox.value = "";
}

function clearAllMessages() {
  chatContent.innerHTML = "";
}
