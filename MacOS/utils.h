//
void __kgmLog(const char* log);
void __kgmStartApp();
void __kgmRunApp();
int  __kgmExitApp();
void* __kgmOpenMainWindow(const char* title, int x, int y, int w, int h);
int __kgmInitMetal(void* wnd);
void __kgmAlert(const char* mst);
