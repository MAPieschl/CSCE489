# AFIT-CSCE489-PROJ1
##### By: Capt Mike Pieschl
##### Date: 16 July 2024

---

#### Table of Contents

**1. Installation**

**2. Usage**

**3. Development**


---

**1. Installation**

Linux installation steps are shown below. Please note that all CSCE489 projects will be cloned into the CSCE489 folder.

    git clone https://github.com/MAPieschl/CSCE489.git

    cd CSCE489_Project1

    make

**2. Usage**

To enter the myshell envrionment, execute:

    ./myshell

This will open the shell environment, which mimics the Linux terminal environment. Once operating in `myshell`, the following commands are available for use:

    create <filename>

PURPOSE:  creates a file in the working directory.

ARGUMENTS:

`<filename>` - any valid UNIX file name

    update <filename> <number of additions> <text>

PURPOSE:  updates a file in the working directory.

ARGUMENTS:

`<filename` - a valid file name that exists in the working directory

`<number of additions>` - the number of times you would like to append `<text>` to the file

`<text>` - the text you would like to append to the file; note that `text` can either be a single term (no spaces) or a phrase (containing spaces) if enclosed by quotations `" "`

    list <filename>

PURPOSE:  updates a file in the working directory.

ARGUMENTS:

`<filename` - a valid file name that exists in the working directory

    dir

PURPOSE:  lists all contents of a directory in the terminal

    halt

PURPOSE:  closes myshell and terminates all associated processes

**3. Development**

If you wish to further expand the commands available in myshell, the following code must be updated:

**shellfuncts.h**

Add your command to the AVAILABLE_CMDS_INDEXER. This will make the command available for `select_command()`:

    enum AVAILABLE_CMDS_INDEXER {CREATE, UPDATE, LIST, DIR, HALT};

Add your command function to the function definitions list.

**shellfuncts.c**

Add a `case` for the name you gave your command in AVAILABLE_CMDS_INDEXER.

Add your command function to match the definition in `shellfuncts.h`.

**myshell.c**

Add the command name to AVAILABLE_CMDS:

    const char AVAILABLE_CMDS[5][7] = {"create", "update", "list", "dir", "halt"};
    
If the command should not be allowed to run in the background (such as a command that lists values to the shell), background processing can be inhibited by adding the command name to NO_BG_PROCESS:

	const char NO_BG_PROCESS[3][7] = {"list", "dir", "halt"};
