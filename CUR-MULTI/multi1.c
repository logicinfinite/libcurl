#include <stdio.h>
#include <curl/curl.h>
#include<string.h>
struct map {
    CURL* handle;
    char* url;
};

//response handling function
struct D
{
	size_t bytes;
	char* resp;
};

size_t func(char* buffer , size_t itemsize, size_t n  , struct D* data)
 {
 size_t bytes = itemsize*n;
 //for(int i=0;i<strlen(buffer);i++)
 	//printf("%c",buffer[i]) ;

 printf("chunk size received in buffer = %zu\n", bytes);
  
 return bytes;
 }
int main(void)
{
CURLM *multi;  //curl multi handle
CURL *curl; // simple curl handle
CURL *curl2; //second simple curl handle
CURLcode res; // res for error codes 
int still_running = 1; //running processes 
curl_global_init(CURL_GLOBAL_ALL); // global init

curl2 = curl_easy_init();  //init easy handle
curl = curl_easy_init(); //init the easy handle
// setting flags to the easy handles
	curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &func); //func will handle the request
	curl_easy_setopt(curl2, CURLOPT_URL, "http://itsabhincsaav.com");
	curl_easy_setopt(curl2, CURLOPT_WRITEFUNCTION, &func);
  struct map entry1,entry2;
  entry1.handle=curl;
  entry1.url = "https://example.com";
  entry1.handle=curl2;
  entry1.url = "http://itsabhincsaav.com";


multi = curl_multi_init(); //init the multi handle
curl_multi_add_handle(multi, curl);
curl_multi_add_handle(multi, curl2); //adding the easy handle to the multi 
struct CURLMsg *m;
  while(still_running) {

    CURLMcode mc; /* curl_multi_poll() return code */
   

    /* we start some action by calling perform right away */
   // printf("before%d\n",still_running);
    mc = curl_multi_perform(multi, &still_running);
    do {
  int msgq = 0;
  m = curl_multi_info_read(multi, &msgq);
  if(m && (m->msg == CURLMSG_DONE)) {
    CURL *e = m->easy_handle;

    CURLcode result;
    long http_code = 0;
     //get the http code returned by the server
        curl_easy_getinfo (e, CURLINFO_RESPONSE_CODE, &http_code);
      printf("--------long http code : ----------%ld\n",http_code );
      printf("Url = %s",e->url);
    
   // curl_multi_remove_handle(multi,e);
   // curl_easy_cleanup(e);
  }
} while(m);
   
    if(mc != CURLM_OK) {
      fprintf(stderr, "curl_multi_wait() failed, code %d.\n", mc);
      break;
    }
    
  }

//cleanups
		curl_multi_remove_handle(multi, curl);
		curl_multi_remove_handle(multi, curl2); //remove the easy handle from the multi one
		curl_easy_cleanup(curl); // cleanup easy
		curl_easy_cleanup(curl2); // cleanup easy
		curl_multi_cleanup(multi); // clean up multi
		curl_global_cleanup(); // global cleanup
  return 0;
}
