//https://discord.com/developers/docs/game-sdk/sdk-starter-guide
//https://discord.com/developers/applications/1179288610139279473/oauth2/general
//https://www.freemonogrammaker.com/68-circle-monograms/
#ifndef DISCORD_RICH_PRESENCE_H
#define DISCORD_RICH_PRESENCE_H

#include <string>

#include <discord/discord.h>

class DiscordRichPresence {
public:
	DiscordRichPresence();
	~DiscordRichPresence();

	void Update();
	void SetDiscordPresence(const std::string& state, const std::string& details, int64_t startTimestamp);
private:
};

#endif 
