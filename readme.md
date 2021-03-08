<h1 align="center">
    <br>
    <a>
        <img src="logo.png" width="200">
    </a>
    <br>
    High-level interfaces for laconic programming in C
    <br>
</h1>

[![CodeFactor](https://www.codefactor.io/repository/github/clowzed/corgasm/badge)](https://www.codefactor.io/repository/github/clowzed/corgasm)

<h1 align="center"> <br>Libraries status<br></h1>

<!---
| lib name    | written | works | tested | docs written |
| ----------- | ------- | ----- | ------ | ------------ |
| argparselib | 🟢    | 🟢  | 🔴   | 🟢         |
| arraylib    | 🟢    | 🟢  | 🟢   | 🔴         |
| listlib     | 🟢    | 🟢  | 🔴   | 🔴         |
| loglib      | 🟢    | 🟢  | 🔴   | 🔴         |
| menulib     | 🟢    | 🟢  | 🔴   | 🔴         |
| stringlib   | 🟢    | 🟢  | 🔴   | 🔴         |
| tablelib    | 🟢    | 🟢  | 🔴   | 🔴         |
| testreprlib | 🟢    | 🟢  | 🔴   | 🔴         |
| avllib      | 🟢    | 🟢  | 🔴   | 🔴         |
| configlib   | 🟢    | 🟢  | 🔴   | 🔴         |
| memtestlib  | 🟢    | 🟢  | 🔴   | 🔴         |

https://www.mobilefish.com/services/markdown/markdown.php
Place converted html table into this div
-->

<div align="center">
    <table align="center">
        <thead align="center">
            <tr>
                <th align="center">lib name</th>
                <th align="center">written</th>
                <th align="center">works</th>
                <th align="center">tested</th>
                <th align="center">docs written</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td align="center">argparselib</td>
                <td align="center">🟢</td>
                <td align="center">🟢</td>
                <td align="center">🔴</td>
                <td align="center">🟢</td>
            </tr>
            <tr>
                <td align="center">arraylib</td>
                <td align="center">🟢</td>
                <td align="center">🟢</td>
                <td align="center">🟢</td>
                <td align="center">🔴</td>
            </tr>
            <tr>
                <td align="center">listlib</td>
                <td align="center">🟢</td>
                <td align="center">🟢</td>
                <td align="center">🔴</td>
                <td align="center">🔴</td>
            </tr>
            <tr>
                <td align="center">loglib</td>
                <td align="center">🟢</td>
                <td align="center">🟢</td>
                <td align="center">🔴</td>
                <td align="center">🔴</td>
            </tr>
            <tr>
                <td align="center">menulib</td>
                <td align="center">🟢</td>
                <td align="center">🟢</td>
                <td align="center">🔴</td>
                <td align="center">🔴</td>
            </tr>
            <tr>
                <td align="center">stringlib</td>
                <td align="center">🟢</td>
                <td align="center">🟢</td>
                <td align="center">🔴</td>
                <td align="center">🔴</td>
            </tr>
            <tr>
                <td align="center">tablelib</td>
                <td align="center">🟢</td>
                <td align="center">🟢</td>
                <td align="center">🔴</td>
                <td align="center">🔴</td>
            </tr>
            <tr>
                <td align="center">testreprlib</td>
                <td align="center">🟢</td>
                <td align="center">🟢</td>
                <td align="center">🔴</td>
                <td align="center">🔴</td>
            </tr>
            <tr>
                <td align="center">avllib</td>
                <td align="center">🟢</td>
                <td align="center">🟢</td>
                <td align="center">🔴</td>
                <td align="center">🔴</td>
            </tr>
            <tr>
                <td align="center">configlib</td>
                <td align="center">🟢</td>
                <td align="center">🟢</td>
                <td align="center">🔴</td>
                <td align="center">🔴</td>
            </tr>
            <tr>
                <td align="center">memtestlib</td>
                <td align="center">🟢</td>
                <td align="center">🟢</td>
                <td align="center">🔴</td>
                <td align="center">🔴</td>
            </tr>
        </tbody>
    </table>
</div>

<h1 align="center">
    <a href = "http://docorgasm.surge.sh/index">Documentation</a>
</h1>
<h4 align="center">
I'm still writing docs for this project <br>
    I'll add them as I'm ready
</h4>

<h1 align="center">
    Installation
</h1>

<p align="center">
    To install this project you can clone this repository into the root folder of your project.
</p>

```bash
cd your-project
git clone https://github.com/clowzed/corgasm.git
```
<p align="center">
    You also can go to releases and download folder without unnecessary files.
</p>

<h1 align="center">
    Collaboration
</h1>
<ol>
    <li> Fork the repository
    <li> Create branch from dev with name of your additional lib
    <li>Create folder with name of your additional lib
    <li> Write your code
    <li>Add unified makefile from root of corgasm and modify dependencies line
    <li>You code must build sucessfully and have no memory leaks (they are checked by drmemory)
    <li>Merge into dev
    <li>Make pull request
    <li> Wait
</ol>

```bash
git clone https://github.com/your-name/corgasm.git
cd corgasm

git checkout dev
git checkout -b newlib

mkdir newlib
cd newlib
code ...

git commit -am "New lib first commit"

git checkout dev
git merge newlib

make pull request on github
```
