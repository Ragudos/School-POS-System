## How to contribute 

1. You need to get a copy of this repository on your editor/computer. Refer to [Copying repository](#copying-repository) section to know how.
2. I am going to setup code blocks that needs to be written. Refer to [Assigning a Task to Yourself](#assigning-a-task-to-yourself) before starting to work on something.
3. After completing the task you took, it's time to commit your work to your forked repository on GitHub. Refer to [Commiting Your Work](#commiting-your-work) section to know how.
4. When your forked repository on GitHub is synced with your local repository (the source code on your computer's code editor is the same as the forked repository's), it's time to add your changes to the [main repository](https://github.com/Ragudos/School-POS-System). Refer to [Creating a Pull Request](#creating-a-pull-request) to know how.

### Copying Repository

**Prerequisites**
- GitHub account
- Git ([Click me to know how to download](https://git-scm.com/))

**Steps**

1.  Create a **fork** of this repository.
2.  Go to your **forked** repository.
3.  Find the dropdown button with the text **"Code"** and click on it.
4.  Copy the link of this repository.
5.  Go to your code editor.
6.  Open a folder in your code editor where you want to place this project in.
7.  Open the terminal of your code editor.
8.  Run:
   
```bash
git clone <replace me with the link of forked repository here> .
```
9. Done

### Assigning a Task to Yourself

Please refer to the example of a function that needs to be populated with code:

**empty function code block**
```c++
double getChange(double payment, double price) {
    throw std::logic_error("unimplemented")
}
```

Replace the existing line of code with the logic of the function.

**Please refer below before writing code**
  1. Go to the [main repository](https://github.com/Ragudos/School-POS-System)
  2. Click on the **Issues** tab
  3. Create an issue
  4. Follow the format that you'll see on the text editor.
  5. Submit the issue

### Commiting Your Work

Now that you have finished your task, you need to add or commit it to the [main repository](https://github.com/Ragudos/School-POS-System).

1. With your code editor open and the project open on your editor, open the terminal of your code editor.
2. You need to add the files you want to commit. To do so, run:

**Note:** This will add every file you changed.
```bash
git add .
```

To add specific files (specify all files' names one-by-one):
```bash
git add <filename1> <filename2>
```

3. After that, you can commit your work. To do so, run:

```bash
git commit -m "<message here>"
```

**Note:** Replace `<message here>` with information about the changes you made. Please make a concise commit message. To know how, refer to [Creating a Commit Message](#creating-a-commit-message).

4. Now that your changes have been committed, you need to push these changes to your forked repository on GitHub. To do so, run:

If it is your first time running this command:
```bash
git push -u origin main
```

If not, you can just run:
```bash
git push
```

5. You should see your changes on your forked repository on GitHub.

### Creating a Pull Request

**Note:** If your forked repository is not in sync with the [main repository](https://github.com/Ragudos/School-POS-System), then you will see something like this:

![image](https://github.com/user-attachments/assets/f78dd353-71b9-4b3b-8746-cec9832b8a89)

If this is the case, you need to discard all your commits to your forked repository first. The steps are:

1. Click the **Sync fork** button
2. If the button **Update branch** is not visible, and instead, the button **Open pull request** is, please discard your commits first.
3. If you discarded your commits, your forked repository should have its state before you pushed all your recent work and synced with the [main repository](https://github.com/Ragudos/School-POS-System)
4. Now, back to your code editor, run:

If you are running this for the first time:
```bash
git pull -u origin main
```

If not:
```bash
git pull
```

5. Your code editor should prompt you to fix **merge conflicts**. ⚠️ **BE CAREFUL AND GET IN TOUCH WITH US BEFORE YOU FIX MERGE CONFLICTS** ⚠️
6. After fixing all **merge conflicts**, just re-run everyhing from the [Committing Your Work](#committing-your-work) section.

After all is fixed, do these steps:

1. Click the **Contribute** button
2. If your forked repository has new commits, then a button **Open pull request** should appear, click it.
3. You will be redirected to a pull request page. I will provide a template for you to fulfill to give me an idea about your changes.
4. Click **Create pull request**
5. Done! More information will be added here

### Creating a Commit Message

1. Please use present tense for all actions. Some examples are:
   - When you added a feature, use `add` at the beginning of a commit message.
   - When you fixed a bug, use `fix` at the beginning of a commit message.
   - When you refactored a source code (changed the names, logic, etc.), use `refactor` at the beginning of a commit message.
   - When you change the formatting of a source code (changed variable names, spaces, etc.), use `format` at the beginning of a commit message.
   
2. After an action, specify what the action defines in 1 sentence.

3. If you made more than one change, use a semi-colon-separated list for each different action.

**Example:**

If I added a function to calculate for a user's change:

```bash
git commit -m "add function getChange() for user"
```
