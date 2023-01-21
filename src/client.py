#!/usr/bin/env python

import asyncio
import websockets


async def echo(websocket):
    async for message in websocket:
        print(message)
        await websocket.send(message)


async def main():
    async with websockets.serve(echo, "localhost", 9002):
        await asyncio.Future()  # run forever


asyncio.run(main())
