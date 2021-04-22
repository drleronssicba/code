#!/usr/bin/env python
# coding=utf-8
#文件操作
#打开-->读写-->关闭
f = open("test.txt", 'w')
#不加选项默认读方式打开
#'w' 写方式打开

# print type(f)

# for line in f:
#     print line[:-1]

# lines = f.readlines()
# for line in lines:
#     print line[:-1]
#
# f.write('aaaaa\n')
# f.writelines(['aaaa\n','bbbb\n','cccc\n'])
#
# #缓冲区 减少io次数，提高速度
#
# f.close()
