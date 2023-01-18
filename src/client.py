import asyncio
import click as cmd
import websockets
from colorama import Fore

PROMPT_COLOR: str = "yellow"
SUCCESS_COLOR: str = "green"
INFO_COLOR: str = "yellow"
FAIL_COLOR: str = "red"
SERVER_MSG_ANNOUNCEMENT: str = "bright_yellow"


async def connect_to_chat(server_ip: str, username: str):
    cmd.secho(cmd.style('Connecting to ' + server_ip, fg=INFO_COLOR, reset=True) + "..")

    try:
        async with websockets.connect("ws://" + server_ip + ":9002") as websocket:
            cmd.secho(cmd.style("Chatserver online", fg=SUCCESS_COLOR))
            while True:
                # Close socket on KeyBoardInterrupt
                user_input: str = cmd.prompt(Fore.CYAN + "You" + Fore.LIGHTCYAN_EX + ": ", prompt_suffix="")

                if len(user_input) == 0:
                    cmd.secho(cmd.style("You cannot send an empty message!", fg=FAIL_COLOR))
                else:
                    await websocket.send(username + ": " + user_input)
                await websocket.recv() # Read messages
    except (websockets.ConnectionClosed, OSError) as exception:
        cmd.secho(cmd.style("Chatserver offline. Stopping client", fg=FAIL_COLOR))


def configure_client():
    username = cmd.style("User", fg='white')
    server_ip = cmd.style("localhost", fg='white')

    cmd.secho(cmd.style("Starting up Carbon-Chat client", fg=INFO_COLOR))
    username = cmd.prompt('Please enter a username', default=username)
    username = cmd.unstyle(username)
    server_ip = cmd.prompt('Hi ' + cmd.style(username, fg="cyan")
                           + ". Which server should I connect to?", default=server_ip)
    server_ip = cmd.unstyle(server_ip)

    asyncio.run(connect_to_chat(server_ip, username))

    # username = cmd.style(username, fg=SERVER_MSG_ANNOUNCEMENT)
    # cmd.secho(username + cmd.style(" joined the chat. Total online users: ", fg=SERVER_MSG_ANNOUNCEMENT)
    #           + cmd.style("2", fg='bright_magenta'))
    # username = cmd.unstyle(username)
    #
    # msg = cmd.style("What are you going to do today?", fg=CHAT_MSG_PREFIX)
    # user = cmd.style("User2", fg=USER_PREFIX)
    # cmd.secho(user + ": " + msg)
    # cmd.prompt(cmd.style("You"))


if __name__ == '__main__':
    try:
        configure_client()
    except cmd.exceptions.Abort:
        cmd.secho(cmd.style("\nExiting client", fg=INFO_COLOR))
