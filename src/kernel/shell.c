#include "text.h"
#include "math.h"
#include "string.h"
#include "retc.h"
#include "vfs/vfs.h"
#include "panic.h"
#include "power.h"
#include "arch/i686/pci.h"
#include "drivers/block/ata.h"
#include "drivers/audio/sb16.h"
#include "drivers/audio/speakers.h"
#include "fs/dataset.h"
#include "bin/zedit.h"
#include "bin/datetime.h"
#include "bin/dsview.h"

static const char* skip_spaces(const char* s)
{
    while (*s == ' ') s++;
    return s;
}

static const char* get_arg1(const char* input)
{
    while (*input && *input != ' ')
        input++;

    return skip_spaces(input);
}
static const char* get_arg2(const char* input)
{
    const char* a1 = get_arg1(input);

    while (*a1 && *a1 != ' ')
        a1++;

    return skip_spaces(a1);
}

static void ls_print(const char* name)
{
    text_print(name);
    text_print("\n");
}

static void shell_newline()
{
    text_print("\n> ");
}


static int get_command(const char* input, char* out_cmd)
{
    int i = 0;

    while (*input && *input != ' ' && i < 31)
    {
        out_cmd[i++] = *input++;
    }

    out_cmd[i] = 0;
    return i;
}

static const char* get_args(const char* input)
{
    while (*input && *input != ' ')
        input++;

    return skip_spaces(input);
}

retc_t shell_execute(const char* cmdline)
{
    char cmd[32];

    get_command(cmdline, cmd);
    const char* args = get_args(cmdline);

    if (strcmp(cmd, "help") == 0)
    {
        text_print("Commands:\n");
        text_print("\n");
        text_print("atapresent................Checks if there's a disk in the system\n");
        text_print("beeptest..................Produces a beep to test if sound works\n");
        text_print("cat.......................Writes what is on a file to the screen\n");
        text_print("calc......................Does a calculation\n");
        text_print("charset...................Displays the character set\n");
        text_print("clear.....................Will crash the system\n");
        text_print("crash.....................Clears the screen\n");
        text_print("createdset................Creates a Dataset\n");
        text_print("daemon....................Daemon :3\n");
        text_print("datetime..................Prints current date and time\n");
        text_print("deldset...................Deletes a dataset\n");
        text_print("dsview....................Dataset explorer\n");
        text_print("hewwo.....................Hewwwo :3\n");
        text_print("indie.....................Indie :3\n");
        text_print("listdir...................Lists all the dirs in a dir\n");
        text_print("listdset..................Lists all datasets on the system\n");
        text_print("listpci...................Lists all PCI devices\n");
        text_print("reset.....................Restarts the system\n");
        text_print("say.......................Prints somthing to the screen\n");
        text_print("shutdown..................Powers off the system\n");
        text_print("zedit.....................Zilo Editor\n");
        return RETC_OK;
    }
    else if (strcmp(cmd, "atapresent") == 0)
    {
        if (ata_detect())
        {
            text_print("ATA Primary Master: Present\n");
        }
        else
        {
            text_print("ATA Primary Master: Not found\n");
        }

        return RETC_OK;
    }
    else if (strcmp(cmd, "sb16test") == 0)
    {
        play_tetris_theme();
        return RETC_OK;
    }
    else if (strcmp(cmd, "cat") == 0)
    {
        vfs_file_t* f = vfs_open(args);

        if (!f)
        {
            text_print("File not found\n");
            return RETC_WRONG_ARG;
        }

        char buf[128];
        int r = vfs_read(f, buf, sizeof(buf)-1, 0);
        buf[r] = 0;

        text_print(buf);
        return RETC_OK;
    }
    else if (strcmp(cmd, "calc") == 0)
    {
        const char* p = args;

        p = skip_spaces(p);
        char op[8];
        int i = 0;

        while (*p && *p != ' ' && i < 7)
            op[i++] = *p++;
        op[i] = 0;

        p = skip_spaces(p);

        int a = 0;
        int sign = 1;

        if (*p == '-') { sign = -1; p++; }

        while (*p >= '0' && *p <= '9')
            a = a * 10 + (*p++ - '0');

        a *= sign;

        p = skip_spaces(p);

        int b = 0;
        sign = 1;

        if (*p == '-') { sign = -1; p++; }

        while (*p >= '0' && *p <= '9')
            b = b * 10 + (*p++ - '0');

        b *= sign;

        int result = 0;
        retc_t rc = RETC_OK;

        if (strcmp(op, "add") == 0) result = math_add(a, b);
        else if (strcmp(op, "sub") == 0) result = math_sub(a, b);
        else if (strcmp(op, "mul") == 0) result = math_mul(a, b);
        else if (strcmp(op, "div") == 0) rc = math_div(a, b, &result);
        else return RETC_WRONG_ARG;

        if (rc == RETC_DIVISION_BY_ZERO)
        {
            text_print("Nice try.\n");
            return rc;
        }

        text_print("= ");
        text_print_int(result);
        text_print("\n");

        return RETC_OK;
    }
    else if (strcmp(cmd, "charset") == 0)
    {
        text_print("VGA charset dump (0-255)\n");

        for (int i = 0; i < 256; i++)
        {
            // Print index for readability
            if (i % 16 == 0)
            {
                text_print("\n");
                text_print_hex8(i);
                text_print(": ");
            }

            text_putc((char)i);
            text_putc(' ');
        }

        text_print("\nDone.\n");
        return RETC_OK;
    }
    else if (strcmp(cmd, "crash") == 0)
    {
        panic_now("Manual crash triggered from shell");
        return RETC_OK; // unreachable
    }
    else if (strcmp(cmd, "clear") == 0)
    {
        text_clear();
        return RETC_OK;
    }
    else if (strcmp(cmd, "createdset") == 0)
    {
        if (*args)
        {
            dataset_create(args, 10); // 10 sectors for now
            return RETC_OK;
        }

        return RETC_WRONG_ARG;
    }
    else if (strcmp(cmd, "hewwo") == 0)
    {
        text_print("Hewwo :3\n");
        return RETC_OK;
    }
    else if (strcmp(cmd, "daemon") == 0)
    {
        text_print("{0x0C}                       :\n");
        text_print("{0x0C}             :      .   ::{0x0F}             DAEMON\n");
        text_print("{0x0C}              ::   .. .   :::{0x0F}          -  Created by Cincoboy06\n");
        text_print("{0x0C}             :::   ...      .::{0x0F}        -  daemon.cincoboy06.xyz\n");
        text_print("{0x0C}            ::.     ....=   :::\n");
        text_print("{0x0C} ........  :::     .  ....::::\n");
        text_print("{0x0C} ... .....::: .  ....... :::::\n");
        text_print("{0x0C}   . .+ . :::::.  ......  .\n");
        text_print("{0x0C}    ...{0x0F}@@..{0x04} ::::::::.. :::: {0x0C}..\n");
        text_print("{0x0C}      .{0x0F}#@@@+{0x04}. ::::: . .......{0x0C}..\n");
        text_print("{0x0C}      .... .  {0x04}... ... .........{0x0C}..\n");
        text_print("{0x0C}         .  ::...::.. {0x04}.   .......{0x0C}..\n");
        text_print("{0x0C}          . ...:::: .. {0x04}::..  .......{0x0C}..\n");
        text_print("{0x0C}                 :.... .... {0x04}..  .... {0x0C}..\n");
        text_print("{0x0C}                     .::  :: .... ....:\n");
        text_print("{0x0C}                             : :: .{0x0F}\n");
        return RETC_OK;
    }
    else if (strcmp(cmd, "datetime") == 0)
    {
        datetime_t t = datetime_get();

        text_print("Current datetime: ");
        text_print_dec(t.hour);
        text_print(":");
        text_print_dec(t.minute);
        text_print(" ");
        text_print_dec(t.day);
        text_print("-");
        text_print_dec(t.month);
        text_print("-");
        text_print_dec(t.year);
        text_print("\n");

        return RETC_OK;
    }
    else if (strcmp(cmd, "deldset") == 0)
    {
        if (*args)
        {
            dataset_delete(args);
            return RETC_OK;
        }

        return RETC_WRONG_ARG;
    }
    else if (strcmp(cmd, "dsview") == 0)
    {
        dsviewer_run();
        return RETC_OK;
    }
    else if (strcmp(cmd, "indie") == 0)
    {
        text_print("{0x0D}              ==:=                      \n");
        text_print("{0x0D}            == . ==        ===. =       {0x0F}      INDIE\n");
        text_print("{0x0D}           = {0x0F}:@@%{0x0D} =  +====:  +* ={0x0F}             - Created by Indie\n");
        text_print("{0x0D}          = {0x0F}+@@@@ ===   : %{0x0F}@@@{0x0D} =+{0x0F}             - @idaawz.bsky.social    \n");
        text_print("{0x0D}         =: {0x0F}@@@@%.=  @  -@={0x0F}{0x0D}  -.=        \n");
        text_print("{0x0D}         = {0x0F}@@@@@ #@@@   +{0x0F}@@@@#{0x0D}  ==      \n");
        text_print("{0x0D}   ==-  -= {0x0F}@@@@% @@@@@%@@@@@@@@@{0x0D}= ==    \n");
        text_print("{0x0D}  =.     {0x0F}. @@@@ @@@@@@@@@@@@@@@@@@{0x0D} -=   \n");
        text_print("{0x0D}  =- {0x0F}@@@@@*#@@ @@@@@@@@@@@@@@@@@@@@{0x0D} :=  \n");
        text_print("{0x0D}   =:{0x0F}*@@@@@#@@@@@@@@@@@@@@@@@@@@@@@@{0x0D} -= \n");
        text_print("{0x0D} +-     {0x0F}%@@@@@@#@@@@@@@@@@#@@@@{0x0D}.:=.   ==\n");
        text_print("{0x0D} == {0x0F}@@@@@:@%#@%#@{0x0D}@. @{0x0F}@@@@@.@@@@@# {0x0D}===== \n");
        text_print("{0x0D}  = {0x0F}@@@==@@ %@%*{0x0D}@ ===%@{0x0F}@@@: %@@@@% {0x0D}=    \n");
        text_print("{0x0D}  == {0x0F}% @@@@ %@+#@{0x0D}@*.+@{0x0F}@@@@@@# %@@@ {0x0D}:=   \n");
        text_print("{0x0D}    =     {0x0F}@ @@=#@@@@@@@@@@@@@ {0x0D}:======   \n");
        text_print("{0x0D}    ====.{0x0F}-% @@=:*@@@@@@@@@@# {0x0D}-=         \n");
        text_print("{0x0D}       +=   {0x0F}#@ {0x0D}====.  =%*  ==           \n");
        text_print("{0x0D}         ==- {0x0F}@{0x0D} ==   -=====              \n");
        text_print("{0x0D}           +=  ==                       \n");
        text_print("{0x0D}             =:=- {0x0F}\n");
        return RETC_OK;
    }
    else if (strcmp(cmd, "listdir") == 0)
    {
        vfs_list(ls_print);
        return RETC_OK;
    }
    else if (strcmp(cmd, "listdset") == 0)
    {
        dataset_list();
        return RETC_OK;
    }
    else if (strcmp(cmd, "listpci") == 0)
    {
        pci_scan();
        return RETC_OK;
    }
    else if (strcmp(cmd, "reset") == 0)
    {
        text_print("Restarting...\n");
        system_reset();
        return RETC_OK;
    }
    else if (strcmp(cmd, "say") == 0)
    {
        if (*args)
        {
            text_print(args);
            text_print("\n");
            return RETC_OK;
        }

        return RETC_WRONG_ARG;
    }
    else if (strcmp(cmd, "shutdown") == 0)
    {
        text_print("Shutting down...\n");
        system_shutdown();
        return RETC_OK;
    }
    else if (strcmp(cmd, "zedit") == 0)
    {
        zedit_run(args);
        return RETC_OK;
    }
    return RETC_UNKNOWN_CMD;
}


