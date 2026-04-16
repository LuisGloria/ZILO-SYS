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
        text_print("Commands: help, clear, hello, say\n");
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
