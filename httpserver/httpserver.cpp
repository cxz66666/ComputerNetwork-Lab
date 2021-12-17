#include "httpserver.h"

httpserver::~httpserver() {
    close(socketFd);
}

httpserver::httpserver(int port, string rootPath) {
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socketFd, (sockaddr *) &sin, sizeof(sin))) {
        perror("socket bind error, exit!");
        exit(1);
    }
    listen(socketFd, MAXCONNECT);
}

void httpserver::mainLoop() const {
    cout << "Begin listen on port " << PORT << endl;
    while (true) {
        sockaddr_in client;
        socklen_t clientLength = sizeof(client);

        int connection = accept(socketFd, (sockaddr *) &client, &clientLength);
        cout << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port) << " connected.\n";
        pthread_t newThread;
        pthread_create(&newThread, NULL, handleThread, &connection);
    }
}

void handleConnect(int cfd) {
    char buffer[MAXBUFFER];
    string httpRequest;
    while (true) {
        int size = recv(cfd, buffer, MAXBUFFER, 0);
        cout << size << endl;
        if (size <= 0) {
            close(cfd);
            return;
        }

        httpRequest.append(buffer, buffer + size);
        if (size < MAXBUFFER) {
            break;
        }
    }
    string reply = replyRequest(httpRequest);
//    cout << reply.size() << " " << reply << endl;
    send(cfd, reply.c_str(), reply.size(), 0);
    close(cfd);
    //这里close交给父函数去处理
    return;
}

string replyRequest(const string &request) {
    regex whitespace("\\s+");
    vector<string> words(sregex_token_iterator(request.begin(), request.end(), whitespace, -1),
                         sregex_token_iterator());
//    for (auto m: words) {
//        cout << m << endl;
//    }
    string ans;
    string type = words[0];
    string path = words[1];
    string http = words[2];
    cout << type << " " << path << " " << http << endl;
    if (type == GET) {
        ans = get(path, http);
    } else if (type == POST) {
        ans = post(path, http, request.substr(request.find("\r\n\r\n", 0) + 4));
    }
    return ans;
}

string get(const string &path, const string &http) {
    regex jpgPattern("\\.(jpg|JPG)");
    regex txtPattern("\\.(txt|TXT)");
    fstream file;
    if (regex_search(path, jpgPattern)) {
        cout << ROOTPATH + path << endl;
        file.open(ROOTPATH + path, ios::in | ios::binary);
    } else {
        cout << ROOTPATH + path << endl;
        file.open(ROOTPATH + path, ios::in);
    }
    if (!file) {
        return makeHttpReply("404 NOT FOUND", 404);
    }
    string fileData((istreambuf_iterator<char>(file)),
                    istreambuf_iterator<char>());

    string contentType;
    if (regex_search(path, jpgPattern)) {
        contentType = "image/jpeg";
    } else if (regex_search(path, txtPattern)) {
        contentType = "text/plain";
    } else {
        contentType = "text/html";
    }
//    cout << fileData << endl;
//    cout << contentType << endl;
    return makeHttpReply(fileData, 200, contentType);
}

string post(const string &path, const string &http, const string &data) {
    if (path != "/dopost") {
        return makeHttpReply("<html><body>Invalid Path</body></html>", 200);
    }
    cout << data << endl;
    cout << path << endl;
    int nowIndex = 0;
    int t = 0;
    map<string, string> mp;
    while (nowIndex < data.size()) {
        string tmpKey, tmpValue;
        tmpKey = data.substr(nowIndex, data.find("=", nowIndex) - nowIndex);
        if ((t = data.find("&", nowIndex)) == string::npos) {
            tmpValue = data.substr(data.find("=", nowIndex) + 1);
            mp[tmpKey] = tmpValue;
            cout << tmpKey << " " << tmpValue << endl;
            break;
        }
        tmpValue = data.substr(data.find("=", nowIndex) + 1, t - data.find("=", nowIndex) - 1);
        nowIndex = t + 1;
        mp[tmpKey] = tmpValue;
        cout << tmpKey << " " << tmpValue << endl;

    }
    if (mp["login"] == USRENAME && mp["pass"] == PASSWORD) {
        return makeHttpReply("<html><body>Login Success</body></html>", 200);
    } else {
        return makeHttpReply("<html><body>Login Fail</body></html>", 200);
    }
}


string makeHttpReply(const string &data, const int &code, const string &contentType) {
    string reply;
    reply.append("HTTP/1.1 ");
    switch (code) {
        case 200:
            reply.append("200 OK\r\n");
            break;
        case 404:
            reply.append("404 Not Found\r\n");
            break;
        default:
            reply.append("404 Not Found\r\n");
            break;
    }
    reply.append("Server: Raynor-TP\r\n");
    reply.append("Content-Type: " + contentType + "\r\n");
    int length = data.size();
    reply.append("Content-Length: " + to_string(length) + "\r\n\r\n");
    if (length > 0) {
        reply.append(data + "\r\n");
    }
    return reply;
}



