##  MyShell - A Custom Linux Terminal Shell

**MyShell** is a lightweight, custom-built terminal shell written in C. It replicates several core features of standard Linux shells like Bash, including command execution, piping, redirection, background jobs, and more — all in a colorful and interactive interface. It feels like you`re using your own shell but in a more simplistic manner with cool colors and extended functionality.

## 🚀 Features

- 🖥️ Custom prompt with username, hostname, and current working directory
- 🔧 Built-in commands:
  - `jobs` – List background jobs
  - `fg <pid>` – Bring background job to foreground
  - `kill <pid>` – Terminate a background job
- 📤 Output redirection (`>`, `>>`) and input redirection (`<`)
- 🔀 Multiple pipe support (`|`)
- 🧵 Background execution using `&`
- 🧠 Memory-efficient job list management
- 🐧 Designed for Linux / WSL environments
- 🐧 Runs all the sudo commands
- 🔧 Supports all `cd` commands and command history
- 📦 Able to launch full-screen terminal apps like vim, htop, etc.
- 🧠 Autocomplete with file suggestions using `alt`
## 📦 Dependencies

- GCC
- GNU Readline library
- A Linux-based OS (Ubuntu/WSL recommended)

### Install dependencies (Ubuntu/Debian):
```bash
sudo apt update
sudo apt install build-essential libreadline-dev cmatrix neofetch
```

## 🛠️ Build Instructions

```bash
git clone https://github.com/yourusername/myshell.git
cd myshell
make
./myshell
```

## 🧪 How to Test

### Run simple commands:
```bash
ls
pwd
date
```

### Try redirection:
```bash
echo "Hello" > file.txt
cat < file.txt
```

### Try piping:
```bash
cat file.txt | grep Hello | wc -l
```

### Run in background:
```bash
sleep 10 &
```

### View jobs:
```bash
jobs
```

### Bring to foreground:
```bash
fg <pid>
```

### Kill a job:
```bash
kill <pid>
```

## ❗ Note

> This shell is developed for **Linux systems only**. If you're using Windows, it's recommended to run this inside **WSL (Windows Subsystem for Linux)**.
