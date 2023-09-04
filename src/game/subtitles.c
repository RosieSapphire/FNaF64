#include <libdragon.h>

#include "game/subtitles.h"

rdpq_font_t *sub_font;

static float night_1_times[] = {
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
};

static const char *night_1_dialogue[] = {
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
	"It's... kind of a legal\nthing, yknow:",
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
	"Uh, the animatronic characters\nhere do get a bit quirky at night",
	"But do I blame them? No.",
	"If I were forced to sing\nthose same stupid songs",
	"for 20 years, and I never got a bath,",
	"I'd probably be a bit\nirritable at night too.",
	"So remember, these characters\nhold a special place in\nthe hearts"
		" of children,",
	"and we need to show them\na little respect. Right? Okay.",
	"So just be aware, the characters\ndo tend to wander a bit.",
	"Uh, they're left in some\nkind of free-roaming mode at night.",
	"Uh... something about their\nservos locking up",
	"if they get turned\noff for too long.",
	"Uh, they used to be able\nto walk around\nduring the day, too",
	"But then there was the\nBite of 87... yeah...",
	"It's amazing that the human\nbody can live without\nthe frontal lobe",
	"Yknow...?",
	"Uh, now, concerning your safety,\nthe only real risk to you,",
	"as the night watch-man here, if any,",
	"is the fact that these characters...\nif they happen to see you",
	"after hours, probably won't \nrecognize you as a person.",
	"They'll most likely see you\nas a metal endoskeleton",
	"without its costume on.",
	"Now, since that's again the rules\nhere at Freddy Fazbear's Pizza,",
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
};

void subtitles_load(void)
{
	sub_font = rdpq_font_load("rom:/debug_font.font64");
	rdpq_font_style(sub_font, 0, &(rdpq_fontstyle_t){
		.color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF),
	});
	rdpq_text_register_font(2, sub_font);
}

static int _subtitles_get_ind(float timer, int night, float offset) {
	switch(night) {
	case 1:
		int highest = 0;
		for(unsigned int i = 0;
				i < sizeof(night_1_times) / sizeof(float);
				i++) {
			if(timer >= night_1_times[i] + offset)
				highest = i;
		}

		return highest;
	
	default:
		return -1;
	}
}

static const char *_subtitles_get_text(float timer, int night, float offset)
{
	switch(night) {
	case 1:
		return night_1_dialogue[
			_subtitles_get_ind(timer, night, offset)];
	
	default:
		return "";
	}
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
