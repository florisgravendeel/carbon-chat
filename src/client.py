import asyncio
import signal

import click as cmd
import websockets
from aioconsole import ainput
from colorama import Fore

PROMPT_COLOR: str = "yellow"
SUCCESS_COLOR: str = "green"
INFO_COLOR: str = "yellow"
FAIL_COLOR: str = "red"
SERVER_MSG_ANNOUNCEMENT: str = "bright_yellow"


async def connect_to_chat(server_ip: str, username: str):
    cmd.secho(cmd.style('Connecting to ' + server_ip, fg=INFO_COLOR, reset=True) + "..")

    try:
        async with websockets.connect("ws://" + server_ip + ":8080") as websocket:
            cmd.secho(cmd.style("Chatserver online", fg=SUCCESS_COLOR))
            # cmd.secho(cmd.style("To disconnect and client stop the client, type: /stop", fg=INFO_COLOR))
            # loop.add_signal_handler(signal.SIGTERM, loop.create_task, websocket.close())
            # asyncio.create_task(open_chat_prompt(websocket, username))
            await asyncio.gather(read_chat_messages(websocket), open_chat_prompt(websocket, username))
            # await asyncio.gather(open_chat_prompt(websocket, username), read_chat_messages(websocket))
            # while True:
            #     cmd.echo(await websocket.recv())
            # asyncio.create_task(read_chat_messages(websocket))

    except (websockets.ConnectionClosed, OSError) as exception:
        cmd.secho(cmd.style("Chatserver offline. Stopping client", fg=FAIL_COLOR))


async def open_chat_prompt(websocket: websockets, username: str):
    while True:
        user_input: str = cmd.prompt(Fore.CYAN + "You" + Fore.LIGHTCYAN_EX + ": ", prompt_suffix="")

        if len(user_input) == 0:
            cmd.secho(cmd.style("You cannot send an empty message!", fg=FAIL_COLOR))
        # elif user_input == "/stop":
        #     cmd
        else:
            await websocket.send(username + ": " + user_input)


async def read_chat_messages(websocket: websockets):
    while True:
        cmd.echo(await websocket.recv())
    # while True:
    #     async for message in websocket:
    #         cmd.secho(message)


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


# if __name__ == '__main__':
#     try:
#         configure_client()
#     except cmd.exceptions.Abort:
#         cmd.secho(cmd.style("\nExiting client", fg=INFO_COLOR))
class PeriodicMessage:
    def __init__(self, message="This is a periodic message. Enter new text to modify it."):
        self.message = message

    async def send_message(self, sleep_delay=3):
        while True:
            print(self.message)
            await asyncio.sleep(sleep_delay)

    async def read_message(self):
        while True:
            print("\nTest test 1 2 3")
            await asyncio.sleep(3)

    async def open_chat_prompt(self):
        while True:
            self.message = await ainput()

    async def main(self):
        await asyncio.gather(self.send_message(), self.open_chat_prompt(), self.read_message())



if __name__ == "__main__":

    periodic_msg = PeriodicMessage()
    loop = asyncio.get_event_loop()
    loop.run_until_complete(periodic_msg.main())
