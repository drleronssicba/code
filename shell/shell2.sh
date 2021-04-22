# read myint
# test $myint -eq 100 # 判断myint的值是否是100
# echo $?

# read myint
# [ $myint -lt 50 ] 
# echo $?

#字符串之间的比较
# read mystring
# [ $mystring == "hello" ]  #[ "X"$mystring == "Xhello" ] 加上一个相同的前缀，因为直接敲回车会出错
# echo $?
#== 之间必须有空格，不然[] 会把中间的当成一个整体
#判断不相等 可以使用!=

# [ -z $mystring ]
# echo $?
#判断mystring 是否为空
#[ -n mystring ]  #判断mystring是否不为空

#文件比较  
#[ -f ./xx.sh ] #判定文件类型是否是普通文件  也可判断文件是否存在 文件不存在  也返回1·
#echo $?
#[ -d xxx ] #判断目录是否存在

#使用 -c 判断某个文件是否是字符设备文件  显示器键盘就是字符设备文件
# [ -c xxx ]
# echo $?

#-d 判断某个文件是否是块设备
# [ -d xxx ]


## 多条件判定 与或非  -a逻辑与(and)  -o逻辑或(or)  !逻辑非
# read mystring
# [ ! "X"$mystring == "Xhehe" ]
# echo $?
# [ $myint lt 50 -o $myint -gt 100 ]
#echo $?

#if 语句
# read myint
# if [ $myint -eq 100 ]
# then
#   echo "hehe"
# fi

# read myint
# # <100 100 - 150 >150 
# if [ $myint -lt 100 ]; then
#   echo "hehe"
# elif [ $myint -lt 150 -a $myint -ge 100 ]; then
#   echo "gread equal 100 and less than 150"
# else
#   echo ">150"
# fi
# if之中亦可以额套if  和c语言类似
#如果不想再if语句中写语句 那么就要用 : （冒号） 代替空语句 不然会报错


# &&表示if then   ||表示if not then
# make clean && make -j16 &&make
# cd /home/lhwei && sh test.sh
# read myint
# [ $myint -eq 100 ] && echo "hehe"  #表示输入100  打印hehe  ||意为条件不成立  则 ...

#case
# read mystring
# 
# case $mystring in 
#   [Ss]start | -s ) # 输入-s等价于 start   [Ss] 表示S和s都可以等价于start  
#     echo "start..."
#     ;; #break
#   'stop' )
#     echo "stop..."
#     ;;
#   * ) #dafault 
#     echo "default..."
#     ;;
# esac

##循环

#for  类似于c语言
# for (( i = 0; i < 10; ++i ))
# do
#   echo $i
# done

#for in
# for i in {0..9}  #前闭后闭
# do
#   echo $i
# done

#for i in {a..z}
#do
#  echo $i
#done

# for i in {1..9} {a..f}
# do
#   echo $i
# done

# for i in {1..9}{a..f}
# do
#   echo $i
# done

## while 循环

# i=0
# while [ $i -lt 5 ]
# do
#   echo $i
#   ((++i))
#     #let ++i
# done

# until  条件为假，继续循环   条件为真，跳出循环  

#死循环
# for ((;;))
# do
#   echo "hehe"
#   sleep 1
# done

# while :
# do
#   echo "hehe"
#   sleep 1
# done

# while true
# do
#   echo "hehe"
#   sleep 1
# done

# until false
# do
#   echo "hehe"
#   sleep 1
# done

# 求1 加到 100
# sum=0
# for i in {1..100}
# do
#   ((sum = $sum + $i))
# done
# 
# echo $sum

#求 1 加到 100 打印出算式
# sum=0
# i=0
# str=''
# for i in {1..100}
# do
#    ((sum = $sum + $i))
#   if [ -z $str ]; then
#     str=$i
#   else
#     str=$str'+'$i
#   fi
# done
# 
# echo  $sum"="$str

#求1 加到 100 中所有基数的和，并打印表达式
# 和上一个程序一样  只需要把++i  变为  i += 2

##特殊变量
# $0 $1 $2 $3 $4 ... 命令行参数
# echo "\$0 -> $0" 
# echo "\$1 -> $1" 
# echo "\$2 -> $2" 
# echo "\$3 -> $3" 
# echo "\$# -> $#" #argc - 1
# echo "\$@ -> $@" # 打印所有的$1...$n
# echo "\$? -> $?" #上一个进程的退出码






























