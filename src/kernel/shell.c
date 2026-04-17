#include "text.h"
#include "string.h"
#include "retc.h"
//#include "time.h"

static const char* skip_spaces(const char* s)
{
    while (*s == ' ') s++;
    return s;
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
        text_print(" help, clear, daemon, hello, indie, say\n");
        text_print("\n");
        text_print("clear.....................Clears the screen\n");
        text_print("daemon....................Daemon :3\n");
        text_print("hello.....................Hello message\n");
        text_print("indie.....................Indie :3\n");
        text_print("say.......................Prints somthing to the screen\n");
        text_print("   |______________________usage: say <text>\n");
        return RETC_OK;
    }
    else if (strcmp(cmd, "clear") == 0)
    {
        text_clear();
        return RETC_OK;
    }
    else if (strcmp(cmd, "hello") == 0)
    {
        text_print("Hello from kernel!\n");
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
    else if (strcmp(cmd, "say") == 0)
    {
        if (*args)
        {
            text_print(args);
            text_print("\n");
            return RETC_OK;
        }

        return RETC_INVALID;
    }

    return RETC_UNKNOWN_CMD;
}


