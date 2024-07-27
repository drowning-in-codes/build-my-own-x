#include "zlib.h"
#include <algorithm>
#include <arpa/inet.h>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <netdb.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <vector>

std::string compressData(const char *input, unsigned long inputLEN,
                         char *&output, unsigned long &outputLEN) {
  z_stream defstream;
  defstream.zalloc = Z_NULL;
  defstream.zfree = Z_NULL;
  defstream.opaque = Z_NULL;

  int ret = deflateInit2(&defstream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15 | 16,
                         8, Z_DEFAULT_STRATEGY);
  if (ret != Z_OK) {
    std::cerr << "Failed to initialize defstream\n";
    return "";
  }

  defstream.next_in = (Bytef *)input;
  defstream.avail_in = inputLEN;
  std::string outStr;
  // 压缩缓冲区的初始大小
  unsigned long compressedSize{32768};
  output = new char[compressedSize];
  do {
    defstream.avail_out = sizeof(output);
    defstream.next_out = reinterpret_cast<Bytef *>(output);

    ret = deflate(&defstream, Z_FINISH);
    if (outStr.length() < defstream.total_out) {
      outStr.append(output, defstream.total_out - outStr.length());
    }
  } while (ret == Z_OK);

  outputLEN = defstream.total_out;

  deflateEnd(&defstream);
  if (ret != Z_STREAM_END) {
    throw(std::runtime_error("Exception during zlib compression: " +
                             std::to_string(ret)));
  }
  return outStr;
}

char *toLower(const char *str) {
  char *lowerStr = new char[strlen(str) + 1];
  lowerStr[strlen(str)] = '\0';
  int n = 0;
  while (n < strlen(str)) {
    lowerStr[n] = std::tolower(str[n]);
    n++;
  }
  return lowerStr;
}

char *strstrInsentive(const char *haystack, const char *needle) {
  char *lowerHaystack = toLower(haystack);
  char *lowerNeedle = toLower(needle);
  return strstr(lowerHaystack, lowerNeedle);
}
std::string parseHeaderField(const char *header, const char *fieldName) {
  char *fieldStart = strstrInsentive(header, fieldName);
  std::string fieldValue;
  if (fieldStart != nullptr) {
    const char *fieldEnd = strstr(fieldStart, "\r\n");
    fieldStart += strlen(fieldName);
    std::string::size_type length = fieldEnd - fieldStart;
    fieldValue.assign(fieldStart, length);
  } else {
    std::clog << "Failed to read " << std::string(fieldName)
              << " from client\n";
  }
  return fieldValue;
}
std::vector<std::string> parseHeaderFieldList(const char *header,
                                              const char *fieldName) {
  std::vector<std::string> fieldValues;
  char *fieldStart = strstrInsentive(header, fieldName);
  std::string fieldValue;
  if (fieldStart != nullptr) {
    fieldStart += strlen(fieldName);
    while (strlen(fieldStart) > 0) {
      const char *fieldEnd = strstr(fieldStart, ", ");
      if (fieldEnd == nullptr) {
        fieldEnd = strstr(fieldStart, "\r\n");
        if (fieldEnd != nullptr) {
          std::string::size_type length = fieldEnd - fieldStart;
          fieldValue.assign(fieldStart, length);
          fieldValues.push_back(fieldValue);
          break;
        }
      } else {
        std::string::size_type length = fieldEnd - fieldStart;
        fieldValue.assign(fieldStart, length);
        fieldValues.push_back(fieldValue);
        fieldStart += length + strlen(", ");
      }
    }
  } else {
    std::clog << "Failed to read " << std::string(fieldName)
              << " from client\n";
  }
  return fieldValues;
}

void handle_client(int client_sock, const std::string dirPath) {
  const int bufsize = 1024;
  char buffer[bufsize] = {0};
  int valread;
  std::string pathName;
  std::string methodName;
  std::string userAgent;
  std::string accept;
  std::string contentType;
  std::string contentLenth;
  std::vector<std::string> acceptEncodings;
  char *requestBody = nullptr;
  if ((valread = recv(client_sock, (void *)&buffer, bufsize, 0)) > 0) {
    buffer[valread] = '\0'; // 截断请求
                            // 找到Request line
    std::clog << "Received buff length: " << valread << std::endl;
    // methodLine = strstr
    const char *methodLine = strchr(buffer, ' ');
    methodName.assign(buffer,
                      static_cast<std::string::size_type>(methodLine - buffer));
    if (methodLine != nullptr) {
      const char *pathStart = methodLine + 1;
      const char *pathEnd = strchr(pathStart, ' ');
      std::string::size_type length = pathEnd - pathStart;
      pathName.assign(pathStart, length);
    } else {
      std::clog << "Failed to read request line from client\n";
    }
    // headers
    char *header = strstr(buffer, "\r\n");
    if (header != nullptr) {
      userAgent = parseHeaderField(header, "User-Agent: ");
      accept = parseHeaderField(header, "Accept: ");
      contentType = parseHeaderField(header, "Content-Type: ");
      contentLenth = parseHeaderField(header, "Content-Length: ");
      acceptEncodings = parseHeaderFieldList(header, "Accept-Encoding: ");
    } else {
      std::clog << "Failed to read headers from client" << std::endl;
    }
    // request body
    char *headerEnd = strstr(header, "\r\n\r\n");
    if (headerEnd) {
      headerEnd = headerEnd + strlen("\r\n\r\n");
      if (strlen(headerEnd) > 0) {
        requestBody = headerEnd;
      }
    }
  }
  std::string echoRoute("/echo/");
  std::string userAgentRoute("/user-agent");
  std::string fileRoute("/files/");
  std::string message;
  std::cout << "path: " << pathName << std::endl;
  std::cout << "User-Agent: " << userAgent << std::endl;
  std::cout << "Accept-Encoding: ";
  for (const auto &encoding : acceptEncodings) {
    std::cout << encoding << ", ";
  }
  std::cout << std::endl;
  if (pathName == "/") {
    message = "HTTP/1.1 200 OK\r\n\r\n";
  } else if (pathName.starts_with(echoRoute)) {
    pathName = pathName.substr(echoRoute.length());
    // std::clog << "path: " << pathName << std::endl;
    // headers
    message = "HTTP/1.1 200 OK\r\n";
    // status line
    auto it = std::find(acceptEncodings.begin(), acceptEncodings.end(), "gzip");
    char *compredData = nullptr;
    std::string outputStr;
    bool compressed = false;
    unsigned long outputLEN{0};
    if (it != acceptEncodings.end()) {
      message += "Content-Encoding: gzip\r\n";
      outputStr = compressData(pathName.c_str(), pathName.length(), compredData,
                               outputLEN);
      compressed = true;
    }
    message += "Content-Type: text/plain\r\n";
    message += "Content-Length: " +
               std::to_string(compressed ? outputLEN : pathName.length()) +
               "\r\n";
    message += "\r\n";
    // response body
    message += compressed ? outputStr : pathName;
  } else if (pathName == userAgentRoute) {
    message = "HTTP/1.1 200 OK\r\n";
    auto it = std::find(acceptEncodings.begin(), acceptEncodings.end(), "gzip");
    if (it != acceptEncodings.end()) {
      message += "Content-Encoding: gzip\r\n";
    }

    message += "Content-Type: text/plain\r\n";
    message += "Content-Length: " + std::to_string(userAgent.length()) + "\r\n";
    message += "\r\n";
    message += userAgent;
  } else if (pathName.starts_with(fileRoute)) {
    std::string fileName = pathName.substr(fileRoute.length());
    if (dirPath.length() > 0) {
      fileName = dirPath + fileName;
    }
    // std::clog << "file: " << fileName << std::endl;
    if (methodName == "POST") {
      // 创建文件
      std::ofstream file(fileName);
      if (file.is_open()) {
        file.write(requestBody, strlen(requestBody));
        file.close();
        message = "HTTP/1.1 201 Created\r\n\r\n";
      } else {
        std::clog << "Failed to open file\n";
      }
    } else {
      std::filesystem::path filePath(fileName);
      if (!std::filesystem::exists(filePath)) {
        message = "HTTP/1.1 404 Not Found\r\n\r\n";
      } else {
        std::ifstream file(fileName);
        if (file.is_open()) {
          auto fileSize = std::filesystem::file_size(fileName);
          char fileBuf[static_cast<size_t>(fileSize)];
          file.read(fileBuf, fileSize);
          message += "HTTP/1.1 200 OK\r\n";
          auto it =
              std::find(acceptEncodings.begin(), acceptEncodings.end(), "gzip");
          if (it != acceptEncodings.end()) {
            message += "Content-Encoding: gzip\r\n";
          }
          message += "Content-Type: application/octet-stream\r\n";
          message += "Content-Length: " + std::to_string(fileSize) + "\r\n";
          message += "\r\n";
          message += fileBuf;
        } else {
          std::clog << "Failed to open file\n";
        }
      }
    }
  } else {
    message = "HTTP/1.1 404 Not Found\r\n\r\n";
  }
  size_t bsent = send(client_sock, message.c_str(), message.length(), 0);

  if (bsent != message.length()) {
    std::cerr << "Failed to send response\n";
  }
  // close(client_sock);
  std::cout << "Client connected\n";
}

int main(int argc, char **argv) {
  // Flush after every std::cout / std::cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  std::string dirPath;
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      std::cout << argv[i] << std::endl;
      if (strcmp(argv[i], "--directory") == 0) {
        dirPath.assign(argv[i + 1]);
      }
    }
  }
  // You can use print statements as follows for debugging, they'll be visible
  // when running tests.
  std::cout << "Logs from your program will appear here!\n";

  // Uncomment this block to pass the first stage
  //
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    std::cerr << "Failed to create server socket\n";
    return 1;
  }

  // Since the tester restarts your program quite often, setting SO_REUSEADDR
  // ensures that we don't run into 'Address already in use' errors
  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) <
      0) {
    std::cerr << "setsockopt failed\n";
    return 1;
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(4221);

  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) !=
      0) {
    std::cerr << "Failed to bind to port 4221\n";
    return 1;
  }

  int connection_backlog = 5;
  if (listen(server_fd, connection_backlog) != 0) {
    std::cerr << "listen failed\n";
    return 1;
  }

  struct sockaddr_in client_addr;
  int client_addr_len;
  std::cout << "Waiting for a client to connect...\n";

  // accept(server_fd, (struct sockaddr *)&client_addr,
  //        (socklen_t *)&client_addr_len);
  while (true) {
    client_addr_len = sizeof(client_addr);
    int client = accept(server_fd, (struct sockaddr *)&client_addr,
                        (socklen_t *)&client_addr_len);
    std::cout << "client port: " << client_addr.sin_port << std::endl;
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    std::cout << "client ip: " << client_ip << std::endl;
    if (client < 0) {
      std::cerr << "Failed to accept client connection\n";
      return 1;
    }
    std::thread clientThread(handle_client, client, dirPath);
    clientThread.detach();
  }
  // // Request line
  // GET                          // HTTP method
  // /index.html                  // Request target
  // HTTP/1.1                     // HTTP version
  // \r\n                         // CRLF that marks the end of the request
  // line
  //
  // // Headers
  // Host: localhost:4221\r\n     // Header that specifies the server's host
  // and port User-Agent: curl/7.64.1\r\n  // Header that describes the
  // client's user agent accept: */*\r\n              // Header that specifies
  // which media types the client can accept \r\n                         //
  // CRLF that marks the end of the headers
  //
  // Request body (empty)
  close(server_fd);
  return 0;
}
