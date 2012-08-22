#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <v8.h>
#include <stdlib.h>
#include <string>


// http://www.dzone.com/snippets/simple-popen2-implementation
#define READ 0
#define WRITE 1

pid_t
popen2(const char *command, int *infp, int *outfp)
{
    int p_stdin[2], p_stdout[2];
    pid_t pid;

    if (pipe(p_stdin) != 0 || pipe(p_stdout) != 0)
        return -1;

    pid = fork();

    if (pid < 0)
        return pid;
    else if (pid == 0)
    {
        close(p_stdin[WRITE]);
        dup2(p_stdin[READ], READ);
        close(p_stdout[READ]);
        dup2(p_stdout[WRITE], WRITE);

        execl("/bin/sh", "sh", "-c", command, NULL);
        perror("execl");
        exit(1);
    }

    if (infp == NULL)
        close(p_stdin[WRITE]);
    else
        *infp = p_stdin[WRITE];

    if (outfp == NULL)
        close(p_stdout[READ]);
    else
        *outfp = p_stdout[READ];

    return pid;
}

v8::Handle<v8::Value> SystemMethod(const v8::Arguments& args) {
    v8::HandleScope scope;

    v8::String::Utf8Value command(args[0]);

    char buf[BUFSIZ+1];
    FILE *fp = popen("ls", "r");
    if (!fp) {
    // TODO: popen
        v8::Local<v8::String> msg = v8::String::New("'popen' failed.");
        v8::ThrowException(v8::Exception::TypeError(msg));
        return scope.Close(v8::Undefined());
    }

    v8::Handle<v8::String::Utf8Value> retval();

    std::string strbuf;
    size_t n;
    while (!!(n=fread(buf, sizeof(char), BUFSIZ, fp))) {
        strbuf.append(buf, n);
    }
    pclose(fp);
    // TODO: handle error on pclose?

    return scope.Close(v8::String::New(strbuf.c_str()));
}

extern "C"
void init(v8::Handle<v8::Object> target) {
  target->Set(v8::String::NewSymbol("system"),
      v8::FunctionTemplate::New(SystemMethod)->GetFunction());
}
NODE_MODULE(hello, init)

