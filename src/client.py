import click as c
import time

from colorama import Back, Style, Fore

username = c.style("User", fg='white')
server_ip = c.style("localhost", fg='white')

if __name__ == '__main__':
    c.secho(c.style("Starting up Carbon-Chat client", fg="yellow"))
    username = c.prompt('Please enter a username', default=username)

    server_ip = c.prompt('Hi ' + c.style(username, fg='cyan')
                         + ". Which server should I connect to?", default=server_ip)
    c.secho(c.style('Connecting to ' + server_ip, fg="yellow", reset=True))
    animation = [
        "[        ]",
        "[=       ]",
        "[===     ]",
        "[====    ]",
        "[=====   ]",
        "[======  ]",
        "[======= ]",
        "[========]",
        "[ =======]",
        "[  ======]",
        "[   =====]",
        "[    ====]",
        "[     ===]",
        "[      ==]",
        "[       =]",
        "[        ]",
        "[        ]"
    ]


    notcomplete = True
    i = 0

    while notcomplete:
        print(Fore.LIGHTGREEN_EX + animation[i % len(animation)], end='\r' + Style.RESET_ALL)
        time.sleep(.1)
        i += 1
        if i == 25:
            break
    c.secho(c.style("Chatserver online", fg='green'))
    username = c.style(username, fg="yellow")
    c.secho(username + c.style(" joined the chat. Total online users: ", fg="bright_yellow")
            + c.style("2", fg='bright_magenta'))
    msg = c.style("What are you going to do today?", fg="bright_cyan")
    c.secho(c.style("User2", fg="cyan") + ": " + msg)
