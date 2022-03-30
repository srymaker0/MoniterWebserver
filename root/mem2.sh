#!/bin/bash
nowtime=`date +"%Y-%m-%d__%H:%M:%S"`
#总量+剩余量
mem_size=`free -m | grep "内存" | awk '{printf("%d %d", $2, $4);}'`
#剩余量
mem_sy=`echo $mem_size | cut -d " " -f2`
#总量
mem_total=`echo $mem_size | cut -d " " -f1`
#当前占用(%)
bnum=`echo "scale=1;($mem_total - $mem_sy) * 100 / $mem_total"|bc`
#占用百分比动态平均值
per_equal=`echo "scale=1;0.3*$1+0.7*$bnum" | bc`
echo "$nowtime ${mem_total}M ${mem_sy}M $bnum% $per_equal%"
