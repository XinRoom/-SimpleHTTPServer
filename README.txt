1. Httpd.exe为运行程序
2. www目录为网页位置
3. data为info.cgi程序的数据目录
4. 建议使用127.0.0.1:8080而非localhost:8080访问  可以直接双击“双击开始浏览.html”访问

===运行环境===
1. vc++2017 runtime library（或许）

-- 2018.06.19 -- XinRoom.cn --

===web服务器 cgi_api===
# cgi是二进制可执行程序
## 目前使用的是进程全局变量，多个连接cgi的请求可能会不可靠，以后会改进
API 环境变量
REMOTE_ADDR     客户端ip地址
REQUEST_METHOD  请求方式“GET”、“POST”
QUERY_STRING    GET方式下的动态查询字符串
POST_DATA       POST方式下的POST数据
URL_PATH        URL，不包含query
REQUEST_HEAD    请求header