First of all Install Git on your computar then ask KI how to install and use it. (it will take you around 5 - 10 minutes )
After that we will be able to code live
Thanks GIT when there is a code error , will be easy to find it with Git.
# Kog_Rob_Projekt

Welcome to the project repository! This project is written in **C**.

---

## 🚀 Getting Started (First Time Only)

If you haven't downloaded the project yet, follow these steps:

1. **Install Git** on your computer if you haven't already.
2. Open your terminal (**Git Bash** on Windows) in your preferred directory.
3. **Clone the repository** by running:
   ```bash
   git clone https://github.com/camus194-cmd/Kog_Rob_Projekt.git
   ```
4. Move into the project folder:
   ```bash
   cd Kog_Rob_Projekt
   ```

---

## 🔄 Daily Workflow (How to work together)

To avoid breaking each other's code, always use this **4-step cycle** every time you work:

### 1. Before you start coding (Download latest updates)
Always download your team's latest changes before touching any file:
```bash
git pull origin main
```

### 2. Check your changes
After writing or fixing your C code, check which files you modified:
```bash
git status
```

### 3. Save your progress locally (Take a snapshot)
Package your clean, working changes and add a short descriptive message:
```bash
git add .
git commit -m "Briefly explain what you did or fixed"
```

### 4. Upload to GitHub (Share with the team)
Send your updates to the cloud so your teammates can see them:
```bash
git push
```

---

## ⚠️ Crucial Rules
* **Do not upload compiled files:** The `.gitignore` file is already configured to block `.exe`, `.ppj`, `.tag`, and `output/` folders. Keep it that way.
* **If git push gets rejected:** It means a teammate uploaded something first. Just run `git pull origin main`, let Git merge the files, and then run `git push` again.
