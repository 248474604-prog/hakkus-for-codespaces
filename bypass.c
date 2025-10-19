#include <stdio.h>
#include <objc/runtime.h>
#include <dispatch/dispatch.h>

// 声明 objc_msgSend 函数
extern id objc_msgSend(id self, SEL op, ...);

__attribute__((constructor))
void init() {
    printf("[Bypass] 强制激活加载!\n");
    
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 3LL * NSEC_PER_SEC), 
                   dispatch_get_main_queue(), ^{
        printf("[Bypass] 开始激活悬浮窗...\n");
        
        Class menuClass = objc_getClass("MenuLoad");
        if (menuClass) {
            printf("[Bypass] 找到MenuLoad类！\n");
            
            SEL sharedSel = sel_registerName("sharedInstance");
            if (class_respondsToSelector(object_getClass((id)menuClass), sharedSel)) {
                id instance = ((id(*)(Class, SEL))objc_msgSend)((id)menuClass, sharedSel);
                if (instance) {
                    SEL showSel = sel_registerName("showMenu:");
                    ((void(*)(id, SEL, id))objc_msgSend)(instance, showSel, (id)1);
                    printf("[Bypass] 已调用 showMenu:\n");
                }
            }
        } else {
            printf("[Bypass] 未找到MenuLoad类\n");
        }
    });
}
