# xv6-labs-2020
MIT 6.s081实验代码
## 提交方式-ssh
### 1. 检查是否存在SSH秘钥
  ```
  ls -al ~/.ssh
  ```
  如果没有显示任何文件，或者没有 id_rsa 和 id_rsa.pub 文件，说明你需要生成一个新的 SSH 密钥对。你可以使用以下命令生成：
  ```
  ssh-keygen -t rsa -b 4096 -C "your_email@example.com"
  ```
  在执行此命令时，请将 your_email@example.com 替换为你的电子邮件地址。在提示你保存密钥时，选择默认位置（按 Enter 键）。然后将公钥内容添加到你的 GitHub 帐户中。

### 2. 在github创建一个新的空仓库 
创建好仓库之后，在虚拟机中使用以下命令来添加名为 "github" 的远程仓库：
  ```
  git remote add github git@github.com:Gwenddi/xv6-labs-2020.git
```
这将在你的 Git 仓库中添加一个名为 "github" 的远程仓库，并将其 URL 设置为 SSH 格式。然后，你可以使用 git remote set-url 命令来设置该远程仓库的 URL：
```
git remote set-url github git@github.com:Gwenddi/xv6-labs-2020.git
```
### 3. 添加git仓库地址
  查看本地仓库的git配置文件，可以看到origin主机名下已经有了对应的上游仓库地址
  ```
   cd xv6-labs-2020/
  cat .git/config
  ```
  **因此我们不要使用origin，可以使用其他主机名对应到github仓库**，例如，我使用github
  ```
  git remote add github 你的仓库地址
  cat .git/config
  ```
### 4. git push
  例如：将实验1用到的util分支推送到github
  ```
  git checkout util
  git push github util:util
  ```
### 5. xv6实验git分支建议
  建议是每个实验创建一个测试分支，例如对于util来说
  ```
  git checkout util         # 切换到util分支
  git checkout -b util_test # 建立并切换到util的测试分支
  ```
  当你在util_test分支中每测试通过一个作业，请提交（git commit）你的代码，并将所做的修改合并（git merge）到util中，然后提交（git push）到github
  ```
  git add .
  git commit -m "完成了第一个作业"
  git checkout util
  git merge util_test
  git push github util:util
  ```
