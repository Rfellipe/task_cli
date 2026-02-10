# task_cli

**Simple and minimal task manager** for the terminal, written in **C**.

## Features

- Add, update, delete tasks
- Mark tasks as **todo**, **in-progress**, or **done**
- List all tasks or filter by status
- Human-readable CLI output
- Zero dependencies
- Written in plain C

## Installation

### Build from source
```bash
git clone https://github.com/your-username/task-cli.git
cd task-cli
make
```

## Usage

```bash
# Adding a new task
task-cli add "Buy groceries"
# Output: Task added successfully (ID: 1)

# Updating and deleting tasks
task-cli update 1 "Buy groceries and cook dinner"
task-cli delete 1

# Marking a task as in progress or done
task-cli mark-in-progress 1
task-cli mark-done 1

# Listing all tasks
task-cli list

# Listing tasks by status
task-cli list done
task-cli list todo
task-cli list in-progress
```

## Contributing
Contributions, suggestions, and bug reports are welcome.
Feel free to open an issue or a pull request.
