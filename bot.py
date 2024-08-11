import discord
import os
from dotenv import load_dotenv
import starboard
import two
import random
import counting
import log
from discord import app_commands
import json
load_dotenv()

TOKEN = str(os.getenv('DISCORD_TOKEN'))


intents = discord.Intents.default()
intents.message_content = True

client = discord.Client(intents=intents)

tree = app_commands.CommandTree(client)
dummy_user = client.fetch_user(0)

@client.event
async def on_ready():
    print(f'logged in as {client.user}!')
    await counting.initialize(client)
    await log.initialize(client)
    await log.info("bot: Hazelbot is online!") 
    await tree.sync(guild=discord.Object(id=1232662729047801928))
    await log.info("bot: tree synced!")

@client.event
async def on_message(message):
    await two.on_message(message, client.user)
    await starboard.on_message(message, client)
    await counting.on_message(message)

    if message.author == client.user:
        return

    await bot_interactions(message) 

@client.event
async def on_message_edit(before, after):
    await two.on_message_edit(before, after, client)
    await counting.on_message_edit(before, after)

@client.event
async def on_raw_reaction_add(payload):
    await starboard.on_reaction(payload, client)

async def bot_interactions(message):
    if "silksong" in message.content.lower():
        options = ["*sigh* bapanada.", "GESSOOOOOOOOOOO", "velmi artrid", "*sigh* apaul", "SHAW", "patamas geo", "DOMA DOMA!! DOMA DOMA DOMA!!!", "RAVA"]
        rand = random.randint(0,len(options) - 1)
        await message.channel.send(options[rand])
    if "step 3" in message.content.lower():
        await message.channel.send("SQUISH")  
    if "hazelbot" in message.content.lower():
        options = ["did someone say my name?", "hey ;]", "hello!!", ":3", "SHUT THE ###### #### ##### ###"]
        rand = random.randint(0, len(options) - 1)
        await message.channel.send(options[rand])

@tree.command(name="cstats",description="Get statistics for counting minigame", guild=discord.Object(id=1232662729047801928))
@app_commands.describe(user="Get stats for a specific user")
async def cstats(interaction, user:discord.User = None):
    save = await counting.get_savefile()
    if user == None:
        content = f"**Highest Count:** {save['st_highest_count']}"
        if save['st_ruinedby'] == "":
            content += "\n**Ruined by**: (count in progress)"
        else:
            content += f"\n**Ruined by:** <@{save['st_ruinedby']}>"
        content += f"\n**Total Counts:** {save['st_counts']}"
        content += f"\n**Total Failures:** {save['st_failures']}"
        embed = discord.Embed(colour = discord.Colour.from_str("#87ffc9"), title = "Counting Stats - Overall", description = content)
        await interaction.response.send_message(embed=embed, ephemeral=False)
    else:
        user_stats = save.get(f"st_user_{user.id}")
        content = ""
        if user_stats == None:
            content = "User has not interacted with the counting system."
        else:
            user_stats = dict(user_stats)
            content = f"**Highest Count:** {user_stats.get('highest_count', 0)}"
            content += f"\n**Total Counts:** {user_stats.get('total_counts', 0)}"
            content += f"\n**Biggest Failure:** {user_stats.get('biggest_failure', 0)}"
            content += f"\n**Total Failures:** {user_stats.get('failures', 0)}"
        embed = discord.Embed(colour = discord.Colour.from_str("#87ffc9"), title = f"Counting Stats - {user.name}", description = content)
        embed.set_thumbnail(url=str(user.display_avatar))
        await interaction.response.send_message(embed=embed)
@tree.command(name="clogsave",description="Log the save file for the counting minigame (for debug purposes)", guild=discord.Object(id=1232662729047801928))
@app_commands.default_permissions(administrator=True)
async def clogsave(interaction):
    save = await counting.get_savefile()
    save_json = json.dumps(save)
    await interaction.response.send_message(f"```json\n{save_json}\n```")


client.run(TOKEN)

input("Waiting for input.")
