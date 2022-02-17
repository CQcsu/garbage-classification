# GIT 使用教程

**git 安装教程**

下载地址：https://git-scm.com/

参考教程：https://blog.csdn.net/mukes/article/details/115693833

**git开始教程**

1.打开git Bash 设置使用Git的姓名和邮箱地址，最后一项让git bash 命令输出具有更好的可读性

```
$ git config --global user.name "Your Name"
$ git config --global user.email "email@example.com"
$ git config --global color.ui auto 
```

设置文件保存在

~~~
~/.gitconfig 输出设置文件
~~~

2.配置ssh公钥

GitHub上链接已有仓库时的认证，通过使用SSH公开密钥认证方式进行.

git Bash中输入命令生成SSH Key

~~~
$ ssh-keygen -t rsa -C "你的邮箱"
第一次回车
然后输入你的密码
重复输入密码
~~~

3.输入命令获得公开密钥id_rsa.pub内容

~~~
$ cat ~/.ssh/id_rsa.pub
~~~

4.复制刚刚生成的public key 添加到你的账户SSH keys中

5.测试SSH key是否配置成功

~~~
ssh -T git@github.com
~~~

**上传文件操作**

打开git Bash cd到想要上传的目录，**用SSH链接本地和远程，https老是报ssl错误**

~~~ #
$ git status # 查看git分支当前状态
$ git init   # 将文件夹设置为本地仓库，这样才能吧本地文件传入github仓库 生成了.git隐藏文件
$ git add .  # 将当前目录所有文件添加到git暂存区
$ git commit -m "测试" # 提交git暂存区的文件。并备注提交信息. -m 后面内容
$ git remote add origin git@github.com:CQcsu/Hello-World.git  # 将github仓库和本地仓库关联
$ git push -u origin master  # 将本地库提交到远程库
~~~

提示出错信息：fatal: remote origin already exists.

删除添加的本地仓库和远程仓库的关联

~~~
$ git remote					# 查看本地添加的远程分支
$ git remote rm origin		# 删除本地指定的远程地址
$ git remote add origin git@github.com:CQcsu/Hello-World.git  # 将github仓库和本地仓库关联
~~~

如果缓存区提交错误文件

~~~
$ git rm -r --cached  # 删除缓存区的所以文
~~~

**下载文件操作**

~~~
打开仓库下载按钮，点击SSH复制
$ git clone git@github.com:CQcsu/Yuntai.git
~~~

**版本管理与回退**

~~~
$ git diff   				# 查看修改情况
$ git log   				# 显示从最近到最远的提交日志
$ git log --pretty=oneline  # 单行显示版本信息
$ git reset   				# 回退到前面的某个版本
~~~

**Github内容更新**

github上只能删除仓库，无法删除文件夹或者文件，所以只能通过命令解决.

~~~
$ git pull origin master    	# 将远程仓库里的项目拉下来
$ dir                      	 	# 查看有哪些文件
$ git add .xx   				# 更新本地操作
$ git commit -m '说明内容' 	     # 提交，添加操作说明
$ git push -u origin master     # 将本次更改更新到github项目上去
~~~

pull命令将远程主机的master分支最新内容拉下来后与当前本地分支直接合并 fetch+merge

不小心把本地pull下载东西删除可以通过命令重新pull下来

~~~
git fetch --all
git reset --hard origin/master
git pull
~~~

**Git分支指令**

主要是开发主线上离开，继续工作

~~~
$ git branch   					 # 列出所有本地分支
$ git branch -r 				 # 列出所有远程分支
$ git branch [branch-name]  	 # 新建一个分支，任然停留在当前分支
$ git checkout -b [branch]       # 新建一个分支，并切换到该分支
$ git merge [branch]   			 # 合并指定分支到当前分支
$ git branch -d [branch-name]    # 删除分支
$ git branch -dr [remote/branch] # 删除远程分支
~~~

git分支简单说明:

~~~
Master分支：这个分支只能从其他分支合并，不能在这个分支直接修改
Develop分支（开发分支）：是我们的主开发分支，包含所有要发布到下一个Release的代码，这个主要合并与其他分支，比如Feature分支
Feature分支（新功能分支）：主要是用来开发一个新的功能，一旦开发完成，我们合并回Develop分支进入下一个Release
Release分支（偏向测试）：发布一个新Release的时候，我们基于Develop分支创建一个Release分支，完成Release后，我们合并到Master和Develop分支
Hotfix分支（紧急bug发布）：在Production发现新的Bug时候，我们需要创建一个Hotfix, 完成Hotfix后，我们合并回Master和Develop分支，所以Hotfix的改动会进入下一个Release
~~~



**git参考教程**

https://www.liaoxuefeng.com/wiki/896043488029600

2.《GItHub 入门与实践》

文档编写：CQ

