#include <iostream>
#include <hidapi.h>

int main() {
    std::cout << "正在初始化 hidapi..." << std::endl;

    if (hid_init() != 0) {
        // cerr 不经过缓冲区直接输出, 一般用于输出错误信息
        std::cerr << "hidapi 初始化失败!" << std::endl;
        return -1;
    }

    std::cout << "Hello DualSenseQAQ!" << std::endl;

    std::cout << "初始化成功! 开始扫描连接的设备..." << std::endl;

    // VID - 供应商识别码
    // PID - 产品识别码
    // hid_enumerate(VID, PID), 传入 0x0, 0x0, 代表找条件为"任何"的所有设备
    // 向系统底层查询, 返回"链表"
    // 只有HID设备(Human Interface Device)
    hid_device_info *devs = hid_enumerate(0x0, 0x0);
    hid_device_info *cur_dev = devs;

    while (cur_dev != NULL) {
        // vendor_id, product_id都是unsigned short类型
        // 必须用unsigned, 不用的话有时会进行符号位扩展, 位运算也会有问题(负数的时候)
        // short 2字节, 16bit, 4个十六进制数(一个16进制数4bit)
        // %X代表输出十六进制(大写字母)
        printf("VID: 0x%04X, PID: 0x%04X\n", cur_dev->vendor_id, cur_dev->product_id);

        // manufacture和product_string都是wchar_t*类型(2字节, UTF-16编码, 16bit)
        // _t代表type, 防止与其他变量名冲突
        // 为了支持多语言, 让一个char里能装下一个字符
        // ls是Long/Wide String
        if (cur_dev->manufacturer_string && cur_dev->product_string) {
            printf("厂商: %ls | 产品: %ls\n", cur_dev->manufacturer_string, cur_dev->product_string);
        } else {
            printf("厂商: 无 | 产品: 无\n");
        }

        cur_dev = cur_dev->next;
    }

    hid_free_enumeration(devs);

    hid_exit();

    std::cout << "扫描结束." << std::endl;

    return 0;
}