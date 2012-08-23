#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <v8.h>
#include <stdlib.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>

v8::Handle<v8::Value> QxMethod(const v8::Arguments& args) {
    v8::HandleScope scope;

    if (!args[0]->IsString()) {
        v8::Local<v8::String> msg = v8::String::New("first argument for qx() should be string");
        v8::ThrowException(v8::Exception::TypeError(msg));
        return scope.Close(v8::Undefined());
    }
    v8::String::Utf8Value command(args[0]);

    char buf[BUFSIZ+1];
    FILE *fp = popen(*command, "r");
    if (!fp) {
        return scope.Close(v8::Undefined()); // ignore and silently return undefined
    }

    v8::Handle<v8::String::Utf8Value> retval();

    std::string strbuf;
    size_t n;
    while (!!(n=fread(buf, sizeof(char), BUFSIZ, fp))) {
        strbuf.append(buf, n);
    }
    if (pclose(fp) == -1) {
        return scope.Close(v8::Undefined()); // ignore and silently return undefined
    }

    return scope.Close(v8::String::New(strbuf.c_str()));
}

v8::Handle<v8::Value> SystemMethod(const v8::Arguments& args) {
    v8::HandleScope scope;

    if (!args[0]->IsString()) {
        v8::Local<v8::String> msg = v8::String::New("first argument for system() should be string");
        v8::ThrowException(v8::Exception::TypeError(msg));
        return scope.Close(v8::Undefined());
    }
    v8::String::Utf8Value command(args[0]);

    char buf[BUFSIZ+1];
    FILE *fp = popen(*command, "r");
    if (!fp) {
        v8::Local<v8::String> msg = v8::String::New("'popen' failed.");
        v8::ThrowException(v8::Exception::TypeError(msg));
        return scope.Close(v8::Undefined());
    }

    v8::Handle<v8::String::Utf8Value> retval();

    std::string strbuf;
    size_t n;
    while (!!(n=fread(buf, sizeof(char), BUFSIZ, fp))) {
        fwrite(buf, sizeof(char), n, stdout);
    }
    int status = pclose(fp);
    int ret;
    if (status != -1 && WIFEXITED(status)) {
        ret = WEXITSTATUS(status);
    } else {
        ret = -1;
    }

    return scope.Close(v8::Integer::New(ret));
}

extern "C"
void init(v8::Handle<v8::Object> target) {
  target->Set(v8::String::NewSymbol("system"),
      v8::FunctionTemplate::New(SystemMethod)->GetFunction());
  target->Set(v8::String::NewSymbol("qx"),
      v8::FunctionTemplate::New(QxMethod)->GetFunction());
}
NODE_MODULE(hello, init)

