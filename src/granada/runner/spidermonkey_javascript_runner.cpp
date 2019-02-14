/**
  * Copyright (c) <2016> granada <afernandez@cookinapps.io>
  *
  * This source code is licensed under the MIT license.
  *
  * Permission is hereby granted, free of charge, to any person obtaining a copy
  * of this software and associated documentation files (the "Software"), to deal
  * in the Software without restriction, including without limitation the rights
  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  * copies of the Software, and to permit persons to whom the Software is
  * furnished to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in
  * all copies or substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
  *
  * Run javascript using Mozilla's JavaScript engine: Spider Monkey.
  * Here spider monkey version 38 is used.
  * https://developer.mozilla.org/en-US/docs/Mozilla/Projects/SpiderMonkey
  */

#include "granada/runner/spidermonkey_javascript_runner.h"

namespace granada{
  namespace runner{

    std::shared_ptr<granada::Functions> SpiderMonkeyJavascriptRunner::functions_ = std::shared_ptr<granada::Functions>(new granada::FunctionsMap());

    // the files containing scripts used in this runner may have the js extensions.
    std::string SpiderMonkeyJavascriptRunner::extensions_arr_[1] = {"js"};
    std::vector<std::string> SpiderMonkeyJavascriptRunner::extensions_(extensions_arr_, extensions_arr_ + sizeof(extensions_arr_)/sizeof(*extensions_arr_));


    JSClass SpiderMonkeyJavascriptRunner::global_class_ = {
        "global",
        JSCLASS_GLOBAL_FLAGS,
        // [SpiderMonkey 38] Replace all stubs but JS_GlobalObjectTraceHook with nullptr.
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    };


    std::string SpiderMonkeyJavascriptRunner::Run(const std::string& _script){
      JSRuntime* rt = JS_NewRuntime(default_numbers::runner_spidermonkey_runtime_maxbytes);
      if (!rt){
        return runner_initialization_error_;
      }

      JSContext* cx = JS_NewContext(rt, default_numbers::runner_spidermonkey_context_stackchunksize);
      if (!cx){
        JS_DestroyRuntime(rt);
        return runner_initialization_error_;
      }

      std::string response = "";
      
      JS_BeginRequest(cx);

      {
        // Scope for our various stack objects (JSAutoRequest, RootedObject), so they all go
        // out of scope before we JS_DestroyContext.

        JS::RootedObject global(cx, JS_NewGlobalObject(cx, &global_class_, nullptr, JS::FireOnNewGlobalHook));
        if (!global){
          response = runner_initialization_error_;
        }else{
          
          // Scope for JSAutoCompartment
          JSAutoCompartment ac(cx, global);
        
          JS_InitStandardClasses(cx, global);
          const char *script = _script.c_str();

          std::shared_ptr<granada::FunctionsIterator> it = functions()->make_iterator();

          while(it->has_next()){
            granada::Function function = it->next();
            JS_DefineFunction(cx, global, function.name.c_str(), FunctionWrapper, 1, 0);
          }

          JS::RootedValue rval(cx);
          JSAutoRequest ar(cx);    
          JS::CompileOptions options(cx);
          bool ok = JS::Evaluate(cx, global, options, script, std::strlen(script), &rval);
          if (ok){
            JSString *str = rval.toString();
            char * bytes = JS_EncodeString(cx, str);
            response = bytes;
            JS_free(cx, bytes);
          }else{
            response = "{\"" + default_strings::runner_error + "\":\"" + default_errors::runner_script_error + "\"}";
          }  
        }
      }

      JS_EndRequest(cx);
      
      JS_DestroyContext(cx);
      JS_DestroyRuntime(rt);

      return response;
    }


    bool SpiderMonkeyJavascriptRunner::FunctionWrapper(JSContext *cx, unsigned argc, JS::Value *vp){

      std::string error;

      char nameBuffer[256];
      const char* name = nameBuffer;
      std::string function_name;

      auto args_a = JS::CallArgsFromVp(argc, vp);
      auto func = JS_ValueToFunction(cx, args_a.calleev());
      
      // get the name of the function we want to call.
      if (func != nullptr) {
        auto funcName = JS_GetFunctionId(func);
        if (funcName != nullptr) {
            auto nameLength = JS_EncodeStringToBuffer(cx, funcName, nameBuffer, sizeof(nameBuffer));
            std::vector<char> vBuffer(nameLength + 1);
            JS_EncodeStringToBuffer(cx, funcName, &vBuffer[0], nameLength);
            vBuffer[nameLength] = '\0';
            name = &vBuffer[0];
            function_name = name;
        }
      }


      JS::CallArgs args = CallArgsFromVp(argc, vp);

      if (function_name.empty()){
        error = default_errors::runner_undefined_function;
      }else{

        // get parameters and call function
        // between the functions of the functions collection.
        JSString* str = args[0].toString();
        char * bytes = JS_EncodeString(cx, str);
        std::string encoded_str = bytes;
        JS_free(cx, bytes);

        web::json::value params;
        try{
          params = web::json::value::parse(encoded_str);
        }catch(const web::json::json_exception& e){
          params = web::json::value::object();
          params[default_strings::runner_error] = web::json::value::string(default_errors::runner_malformed_parameters);
          params[default_strings::runner_error_description] = web::json::value::string(default_error_descriptions::runner_malformed_parameters);
        }

        std::string response_str;

        // recursive javascript eval is not allowed,
        // a thread is created to bypass this problem.
        granada::function_json_json fn = SpiderMonkeyJavascriptRunner::functions_->Get(function_name);
        pplx::create_task([&fn,&params,&response_str]{
          // call the function.
          try{
            response_str = fn(params).serialize();
          }catch(const web::json::json_exception& e){
            response_str = "{}";
          }
        }).wait();
        

        JS::RootedString name_str(cx, JS_NewStringCopyZ(cx, response_str.c_str()));
        if (!name_str){
          return false;
        }
        args.rval().setString(name_str);
      }

      if (!error.empty()){
        JS::RootedString name_str(cx, JS_NewStringCopyZ(cx, error.c_str()));
        if (!name_str){
          return false;
        }
        args.rval().setString(name_str);
      }

      return true;
    }


    void SpiderMonkeyJavascriptRunner::Init(){
      JS_Init();
    }

    void SpiderMonkeyJavascriptRunner::Destroy(){
      JS_ShutDown();
    }
  }
}
