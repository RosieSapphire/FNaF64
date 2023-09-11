#include <libdragon.h>

#include "game/subtitles.h"

#define MAX_LINES 64

static const int night_line_nums[5] = {63, 25, 19, 20, 13};

rdpq_font_t *sub_font;

static float night_times[5][MAX_LINES] = {
	{ // night 1
		0, // nothing
		3.4f, // ringing
		18.9f, // hello
		21.7f, // wanted to record
		24.7f, // to help you
		27.5f, // I worked before
		31.1f, // finishing up
		34.2f, // overwhelming
		37.8f, // nothing to worry
		40.7f, // do fine
		42.4f, // focus
		44.4f, // first week
		46.5f, // lets see
		48.4f, // first introductory
		51.5f, // supposed to read
		53.1f, // legal thing
		55.8f, // welcome to freddy's
		58.9f, // magical place
		61.9f, // where fantasy
		64.6f, // not responcible
		69.1f, // upon discovery
		71.5f, // missing person
		74.4f, // or as soon
		78.3f, // carpets replaced
		81.3f, // might sound bad
		83.6f, // nothing to worry
		86.4f, // a bit quirky~
		90.9f, // blame them? no
		92.9f, // if i were forced
		96.3f, // 20 years never bath
		98.5f, // id be irritable
		101.1f, // special place
		105.2f, // respect
		109.1f, // just be aware
		114.0f, // free-roaming mode
		117.9f, // servous locking up
		120.5f, // turned off too long
		122.6f, // used to be able
		125.5f, // bite of 87
		129.8f, // it's amazing that
		133.5f, // yknow?
		134.4f, // now concerning
		138.5f, // as night watch
		140.7f, // is that these chars
		144.4f, // after hours
		148.9f, // most likely recognize
		152.2f, // without its costume
		153.9f, // since against rules
		157.6f, // they'll probably try
		163.6f, // might not be so bad
		168.6f, // crossbeams, wires, etc
		172.6f, // especially around face
		174.5f, // so you could imagine
		178.3f, // could cause a bit of
		182.4f, // the only parts of you
		185.1f, // would be your eyes
		189.3f, // yeah they don't
		192.4f, // but hey
		194.4f, // i'll chat tomorrow
		195.9f, // and only close doors
		201.0f, // gotta conserve power
		202.7f, // alright, goodnight
		204.2f, // n/a
	},

	{ // night 2
		0.0f, // nothing
		3.6f, // ringing
		18.7f, // hello
		21.0f, // if you're hearing
		25.7f, // i won't talk quite as long
		28.6f, // freddy and his friends
		32.9f, // it might be a good idea
		36.7f, // just to make sure
		40.1f, // interestingly enough
		45.1f, // I've heard he becomes
		49.5f, // i guess that's one more
		53.0f, // I also want to emphasize
		57.5f, // there are blindspots
		60.8f, // and those blindspots
		64.8f, // so if you can't find something
		67.6f, // or someone on your cameras
		71.7f, // you might only have
		74.8f, // not that you'd be in danger
		80.3f, // also check on the curtain
		85.4f, // the character in there seems
		88.3f, // more active if the cameras
		92.4f, // I guess he doens't like being
		95.5f, // anyway i'm sure you have everything
		98.7f, // talk to you soon!
		100.4f, // n/a
	},

	{ // night 3
		0.0f, // nothing
		5.2f, // ringing
		19.7f, // hello hello
		21.0f, // hey you're doing great
		25.1f, // i mean they usually
		29.4f, // i'm not implying they died
		34.0f, // anyway I better not take up
		37.4f, // things start getting real tonight
		39.7f, // hey listen I had an idea
		42.7f, // if you happen to get caught
		47.7f, // try playing dead go limp
		51.3f, // then there's a chance that
		56.3f, // then again if they think you're
		59.8f, // stuff a metal endoskeleton into you
		62.3f, // wonder how that would work
		64.0f, // yeah nevermind, just don't get caught
		68.4f, // okay I'll leave you to it
		71.4f, // see you on the flipside
		72.5f, // n/a
	},

	{ // night 4
		0.0f, // nothing
		3.3f, // ringing
		17.6f, // hello hello
		19.4f, // hey wow day 4 i knew
		23.3f, // i may not be around
		26.7f, // it's been a bad night here
		30.5f, // I'm kinda glad I recorded
		35.0f, // uh when I did
		36.3f, // hey do me a favor
		39.9f, // you could check those suits
		43.7f, // I'm gonna try and hold out
		49.1f, // i always wondered
		52.7f, // *music box* oh no
		55.1f, // *moaning*
		56.4f, // oh no...
		57.3f, // *loud banging*
		58.3f, // *robot scream*
		58.8f, // *signal cuts out*
		60.4f, // *phone hangs up*
		62.0f, // n/a
	},

	{ // night 5
		0.0f, // nothing
		3.9f, // ringing
		17.3f, // *static*
		18.8f, // *demonic muttering*
		26.7f, // *deep scream*
		27.0f, // *demonic muttering*
		30.0f, // *deep scream*
		30.3f, // *demonic muttering*
		33.3f, // *deep scream*
		33.6f, // *demonic muttering*
		34.6f, // *high-pitch screaming*
		35.5f, // *phone hangs up*
		36.3f, // n/a
	},
};

static const char *night_dialogue[5][MAX_LINES] = {
	{ // night 1
		"",
		"*phone ringing*",
		"Hello? Uh, hello?",
		"Uh, I wanted to record a\nmessage for you",
		"To help you get settled\nin on your first night.",
		"Um, I actually worked in\nthat office before you.",
		"I'm finishing up my last\nweek now as a matter of fact.",
		"So, I know it can be a bit\noverwhelming,",
		"But I'm here to tell you\nthere's nothing to worry about",
		"You'll do fine!",
		"So let's just focus",
		"on getting you through\nyour first week.",
		"Okay, uh let's see...",
		"First there's an introductory\ngreeting from the company...",
		"That I'm supposed to read.",
		"It's... kind of a legal\nthing, y'know:",
		"Um, \"Welcome to Freddy\nFazbear's Pizza\"",
		"\"A magical place for kids\nand grownups alike\"",
		"\"Where fantasy and fun\ncome to life\"",
		"\"Fazbear Entertainment\nis not responsible for\n"
			"damage to property or person\"",
		"\"Upon discovering that damage\nor death has occured\"",
		"\"A missing person report will\nbe filed within 90 days\"",
		"\"Or as soon as property and\npremises have been thoroughly\n"
			"cleaned and bleached\"",
		"\"And the carpets have been replaced\"\nBlah, blah blah.",
		"Now that might sound bad, I know.",
		"But there's really nothing\nto worry about",
		"Uh, the animatronic characters\n"
			"here do get a bit quirky at night",
		"But do I blame them? No.",
		"If I were forced to sing\nthose same stupid songs",
		"for 20 years, and I never got a bath,",
		"I'd probably be a bit\nirritable at night too.",
		"So remember, these characters\nhold a special place in\n"
			"the hearts of children,",
		"and we need to show them\na little respect. Right? Okay.",
		"So just be aware, the characters\ndo tend to wander a bit.",
		"Uh, they're left in some\nkind of free-roaming mode at night.",
		"Uh... something about their\nservos locking up",
		"if they get turned\noff for too long.",
		"Uh, they used to be allowed\nto walk around\n"
			"during the day, too",
		"But then there was the\nBite of 87... yeah...",
		"It's amazing that the human\nbody can live without\n"
			"the frontal lobe",
		"Yknow...?",
		"Uh, now, concerning your safety,\nthe only real risk to you,",
		"as the night watch-man here, if any,",
		"is the fact that these characters...\n"
			"if they happen to see you",
		"after hours, probably won't \nrecognize you as a person.",
		"They'll most likely see you\nas a metal endoskeleton",
		"without its costume on.",
		"Now, since that's again the rules\n"
			"here at Freddy Fazbear's Pizza,",
		"They'll probably try to... forcefully\n"
			"stuff you inside a Freddy Fazbear Suit...",
		"Um... Now that wouldn't be so bad\n"
			"if the suits themselves weren't filled with...",
		"Cross-beams, wires, and\nanimatronic devices.",
		"Especially around the facial area",
		"So you could imagine how having\n"
			"your head forcefully pressed\n"
			"inside one of those",
		"Could cause a bit of\n"
			"discomfort... and death...",
		"Uh, the only parts of you that would\n"
			"likely see the light of day again",
		"would be your eyeballs and\n"
			"teeth, which would pop out the\n"
			"front of the mask...",
		"... Y-Yeah they don't tell you\n"
			"these things when you sign up... ",
		"But hey, first day's\n"
			"still a breeze!",
		"I'll chat with you tomorrow.",
		"Uh, check those cameras, and remember\n"
			"to close the doors only if absolutely necessary.",
		"Gotta conserve power.",
		"Alright, goodnight.",
		""
	},

	{ // night 2
		"",
		"*phone ringing*",
		"Uh, hello! Hello! Uh...",
		"Well, if you're hearing this, you made\n"
			"it to day 2. Uh, congrats!",
		"Uh, I-I won't talk quite as long\n"
			"this time since...",
		"Freddy and his friends tend to become\n"
			"more active as the week progresses.",
		"Uh... it might be a good idea to peak\n"
			"at those cameras while I talk,",
		"just to make sure everyone's\n"
			"in their proper place, y'know.",
		"Uh, interestingly enough, Freddy himself\n"
			"doesn't come off stage very often.",
		"I've heard he becomes a lot more\n"
			"active in the dark though, so hey...",
		"I guess that's one more reason\n"
			"not to run out of power, right?",
		"Uh, I also want to emphasize the\n"
			"importance of using your door lights.",
		"Uh, there are blind spots\n"
			"in your camera view.",
		"And those blindspots happen to\n"
			"be right outside your doors.",
		"So, i-if you can't find something...",
		"... or someone on your cameras, uh\n"
			"be sure to check the door lights.",
		"Uh... you might only have a\n"
			"few seconds to react.",
		"Uh, not that you'd be in any\n"
			"danger of course, I'm not implying that.",
		"Uh, also, uh, check on the curtain\n"
			"in Pirate Cove from time to time.",
		"The character in there seems unique,\n"
			"in that he becomes...",
		"more active if the cameras remain\n"
			"off for long periods of time.",
		"I guess he doesn't like being watched.\n"
			"I don't know.",
		"Uh, anyway, I'm sure you have\n"
			"everything under control.\n",
		"Uh, talk to you soon!\n",
		""
	},

	{ // night 3
		"",
		"*phone ringing*",
		"Hello- hello!",
		"Hey, you're doing great! Most\n"
			"people don't last this long!",
		"I mean... y'know, they usually move\n"
			"on to other things by now...",
		"Uh, I'm not implying that they died...\n"
			"that's not what I meant...",
		"Uh, anyway, I better not\n"
			"take up too much of your time.\n",
		"Things start getting real tonight...\n",
		"Uh, hey listen, I-I had an idea!\n",
		"If you happen to get caught, and want to\n"
			"avoid getting stuffed into a Freddy suit,\n",
		"Uh, try playing dead; y'know, go limp.\n",
		"Then there's a chance that uh...\n"
			"maybe they'll think that you're an\n"
			"empty costume instead.\n",
		"... then again if they think you're an\n"
			"empty costume, they might try to...\n",
		"stuff a metal skeleton into you...\n",
		"Wonder how that would work...\n",
		"... Yeah nevermind, scratch that,\n"
			"it's best just not to get caught.\n",
		"Okay, I'll leave you to it.\n",
		"See you on the flip-side!\n",
		""
	},

	{ // night 4
		"",
		"*phone ringing*\n",
		"Hello- hello, hey!\n",
		"Hey wow, day 4... I knew\n"
			"you could do it. Uh, hey listen...\n",
		"I may not be around to send\n"
			"you a message tomorrow.\n",
		"It's... it's been a bad\n"
			"night here... for me...\n",
		"Um... I'm kinda glad that I\n"
			"recorded my messages for you...\n",
		"Uh... when I did...\n",
		"Uh, hey, do me a favor, uh\n"
			"maybe sometime...",
		"you could check inside those\n"
			"suits... in the backroom.\n",
		"I'm gonna try to hold out until someone...\n"
			"checks... maybe it won't be so bad...\n",
		"I-I always wondered what was in\n"
			"all those empty heads... back there...\n",
		"*music box* Oh no...",
		"*moaning*",
		"Oh no...",
		"*loud banging*",
		"*robot scream*",
		"*signal cuts out*",
		"*phone hangs up*",
		""
	},

	{ // night 5
		"",
		"*phone ringing*",
		"*static*",
		"*demonic muttering*",
		"*deep scream*",
		"*demonic muttering*",
		"*deep scream*",
		"*demonic muttering*",
		"*deep scream*",
		"*demonic muttering*",
		"*high-pitch screaming*",
		"*phone hangs up*",
		""
	},
};

void subtitles_load(void)
{
	sub_font = rdpq_font_load("rom:/custom/debug_font.font64");
	rdpq_font_style(sub_font, 0, &(rdpq_fontstyle_t){
		.color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF),
	});
	rdpq_text_register_font(2, sub_font);
}

static int _subtitles_get_ind(float timer, int night, float offset) {
	int highest = 0;
	for(int i = 0; i < night_line_nums[night - 1]; i++) {
		if(timer >= night_times[night - 1][i] + offset)
			highest = i;
	}

	return highest;
}

static const char *_subtitles_get_text(float timer, int night, float offset)
{
	return night_dialogue[night - 1]
		[_subtitles_get_ind(timer, night, offset)];
	
}

void subtitles_draw(float timer, int night, float offset)
{
	const rdpq_textparms_t parms = {
		.align = 1, .valign = 1,
		.width = 320,
	};
	rdpq_text_printf(&parms, 2, 0, 48,
			_subtitles_get_text(timer, night, offset));
}

void subtitles_unload(void)
{
	rdpq_font_free(sub_font);
}
