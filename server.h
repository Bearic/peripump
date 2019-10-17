#ifndef SERV
#define SERV

String getContentType(String filename);
bool handleFileRead(String path);
void handleRoot();
void handleNotFound();

#endif
