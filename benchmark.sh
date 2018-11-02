#!/bin/bash
# 从命令行获取块名称
blocknames=$1
# 声明常量
execfile=./bin/DII_STT.ex
parafile=log.txt
# 正演参数块 名称为forward
if [[ $blocknames == "forward-b2" ]]; then
# 声明程序参数文件内容并通过cat保存在parafile
cat <<- EOF > $parafile
	runtype=forward
	datatype=gravity
	# 模型参数说明
	model-file=doc/benchmark2/model-benchmark2.msh
	basement-topo="vertex value: bottom"
	forward-topo="vertex value: icosa"
	density-distribution="vertex value: rho"
	# 正演参数
	refer-radius=1e+5/1e+5
	#obserin-para=-180/5/180/-90/5/90/100
	obserin-para=vert/10000
	obserout-para=doc/benchmark2/modelObs_grav.txt
	noise-dev=0.2
	# 基准面重力值
	basement-file=doc/benchmark2/modelBase_grav.txt
EOF
# 运行程序
$execfile $parafile

# 正演参数块 名称为forward
elif [[ $blocknames == "forward-b1" ]]; then
# 声明程序参数文件内容并通过cat保存在parafile
cat <<- EOF > $parafile
	runtype=forward
	datatype=gravity
	# 模型参数说明
	model-file=doc/benchmark/model-benchmark.msh
	basement-topo="vertex value: baseTopo"
	forward-topo="vertex value: referTopo"
	density-distribution="vertex value: rho"
	# 正演参数
	refer-radius=1000/1000
	#obserin-para=vert/100
	obserin-para=30/0.5/60/20/0.5/50/100
	obserout-para=doc/benchmark/modelObs_grav_reg.txt
	noise-dev=1e-3
	# 基准面重力值
	basement-file=doc/benchmark/modelBase_grav_reg.txt
EOF
# 运行程序
$execfile $parafile

# 反演参数块 名称为inversion
elif [[ $blocknames == "inversion-b2" ]]; then
# 声明程序参数文件内容并通过cat保存在parafile
cat <<- EOF > $parafile
	runtype=inversion
	datatype=gravity
	debugModel=0
	strongCondition=0
	model-file=doc/benchmark2/model-benchmark2.msh
	dk-file=doc/benchmark2/gradient.msh
	basement-topo="vertex value: bottom"
	initial-topo="vertex value: referModel"
	upper-boundary-topo="vertex value: upBoundary"
	density-distribution="vertex value: rho"
	basement-file=doc/benchmark2/modelBase_grav.txt
	tempRes-file=doc/benchmark2/tempRes.msh
	msh-file=doc/benchmark2/invertTopography_grav.msh
	refer-radius=1e+5/1e+5
	obserin-para=doc/benchmark2/modelObs_grav.txt
	c1=1e-4
	c2=0.95
	ak=1e+7
EOF
# 运行程序
$execfile $parafile

# 反演参数块 名称为inversion
elif [[ $blocknames == "inversion-b1" ]]; then
# 声明程序参数文件内容并通过cat保存在parafile
cat <<- EOF > $parafile
	runtype=inversion
	datatype=gradient
	debugModel=1
	strongCondition=0
	model-file=doc/benchmark/model-benchmark.msh
	dk-file=doc/benchmark/gradient.msh
	basement-topo="vertex value: baseTopo"
	initial-topo="vertex value: last time"
	density-distribution="vertex value: rho"
	basement-file=doc/benchmark/modelBase_grad.txt
	tempRes-file=doc/benchmark/tempRes.msh
	msh-file=doc/benchmark/invertTopography_grad.msh
	refer-radius=1000/1000
	obserin-para=doc/benchmark/modelObs_grad.txt
	c1=1e-4
	c2=0.95
	ak=1
EOF
# 运行程序
$execfile $parafile

fi