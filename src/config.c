/***************************************************************************

    config.c

    Configuration file I/O.

***************************************************************************/

#include "driver.h"
#include "config.h"
#include "common.h"
//#include "expat.h"


#define DEBUG_CONFIG		0



/*************************************
 *
 *  Constants
 *
 *************************************/

#define CONFIG_VERSION			10
#define TEMP_BUFFER_SIZE		4096
#define MAX_DEPTH				8
#define MAX_MIXER_CHANNELS		100

enum
{
	FILE_TYPE_GAME,				/* game-specific config file */
	FILE_TYPE_DEFAULT,			/* default keys config file */
	FILE_TYPE_CONTROLLER		/* controller-specific config file */
};



/*************************************
 *
 *  Type definitions
 *
 *************************************/

struct config_port
{
	struct config_port *	next;				/* next in the list */
	int						type;				/* type attribute */
	int						player;				/* player attribute */
	int						mask;				/* mask for the port */
	int						defvalue;			/* default value of the port */
	int						value;				/* value of the port */
	int						keydelta;			/* keydelta attribute */
	int						centerdelta;		/* centerdelta attribute */
	int						sensitivity;		/* sensitivity attribute */
	int						reverse;			/* reverse attribute */
	input_seq_t				defseq[3];			/* default sequences */
	input_seq_t				newseq[3];			/* current sequences */
};


struct config_counters
{
	int						coins[COIN_COUNTERS];/* array of coin counter values */
	int						tickets;			/* tickets dispensed */
};


struct config_mixer
{
	float					deflevel[MAX_MIXER_CHANNELS];/* array of mixer values */
	float					newlevel[MAX_MIXER_CHANNELS];/* array of mixer values */
};


struct config_data
{
	int						version;			/* from the <mameconfig> field */
	UINT8					ignore_game;		/* are we ignoring this game entry? */
	UINT8					loaded_count;		/* number of systems we loaded */
	input_code_t			remap[__code_max];	/* remap table */
	struct config_port *	port;				/* from the <port> field */
	struct config_counters  counters;			/* from the <counters> field */
	struct config_mixer		mixer;				/* from the <mixer> field */
};


//struct config_parse
//{
//	XML_Char				tag[1024];			/* combined tags */
//	XML_Char				data[1024];			/* accumulated data on the current tag */
//	int						datalen;			/* accumulated data length */
//	int						seq_index;			/* current index in the sequence array */
//};


struct config_file
{
	mame_file *				file;				/* handle to the file */
	int						filetype;			/* what type of config file is this? */
	struct config_data		data;				/* the accumulated data */
	//struct config_parse		parse;				/* parsing info */
};



/*************************************
 *
 *  Global variables
 *
 *************************************/

static struct config_file curfile;

//static const char *seqtypestrings[] = { "standard", "decrement", "increment" };



/*************************************
 *
 *  Prototypes
 *
 *************************************/

static void cleanup_file(void);

static int config_load_xml(void);
static int config_save_xml(void);

static int apply_ports(const struct InputPort *input_ports_default, struct InputPort *input_ports);
static int apply_default_ports(const struct InputPortDefinition *input_ports_default_backup, struct InputPortDefinition *input_ports_default);
static int apply_counters(void);
static int apply_mixer(void);

static int build_ports(const struct InputPort *input_ports_default, const struct InputPort *input_ports);
static int build_default_ports(const struct InputPortDefinition *input_ports_default_backup, const struct InputPortDefinition *input_ports_default);
static int build_counters(void);
static int build_mixer(void);



/*************************************
 *
 *  Game data save/load
 *
 *************************************/

int config_load(const struct InputPort *input_ports_default, struct InputPort *input_ports)
{
	int result;

	/* clear current file data to zero */
	memset(&curfile, 0, sizeof(curfile));

	/* open the config file */
	curfile.file = mame_fopen(Machine->gamedrv->name, 0, FILETYPE_CONFIG, 0);
	if (!curfile.file)
		return 0;
	curfile.filetype = FILE_TYPE_GAME;

	/* load the XML */
	result = config_load_xml();
	mame_fclose(curfile.file);
	if (!result)
	{
		cleanup_file();
		return 0;
	}

	/* first apply counters and mixer data; it's okay if they fail */
	apply_counters();
	apply_mixer();

	/* return true only if we can apply the port data */
	result = apply_ports(input_ports_default, input_ports);
	cleanup_file();
	return result;
}


void config_save(const struct InputPort *input_ports_default, const struct InputPort *input_ports)
{
	/* clear current file data to zero */
	memset(&curfile, 0, sizeof(curfile));

	/* build the various components in memory */
	if (!build_ports(input_ports_default, input_ports))
		goto error;
	if (!build_counters())
		goto error;
	if (!build_mixer())
		goto error;

	/* open the config file */
	curfile.file = mame_fopen(Machine->gamedrv->name, 0, FILETYPE_CONFIG, 1);
	if (!curfile.file)
		return;
	curfile.filetype = FILE_TYPE_GAME;

	/* save the XML */
	config_save_xml();
	mame_fclose(curfile.file);

error:
	cleanup_file();
}



/*************************************
 *
 *  Default control data save/load
 *
 *************************************/

int config_load_default(const struct InputPortDefinition *input_ports_backup, struct InputPortDefinition *input_ports)
{
	int result;

	/* clear current file data to zero */
	memset(&curfile, 0, sizeof(curfile));

	/* open the config file */
	curfile.file = mame_fopen("default", 0, FILETYPE_CONFIG, 0);
	if (!curfile.file)
		return 0;
	curfile.filetype = FILE_TYPE_DEFAULT;

	/* load the XML */
	result = config_load_xml();
	mame_fclose(curfile.file);
	if (!result)
		return 0;

	/* return true only if we can apply the port data */
	result = apply_default_ports(input_ports_backup, input_ports);
	cleanup_file();
	return result;
}


void config_save_default(const struct InputPortDefinition *input_ports_backup, const struct InputPortDefinition *input_ports)
{
	/* clear current file data to zero */
	memset(&curfile, 0, sizeof(curfile));

	/* build the various components in memory */
	if (!build_default_ports(input_ports_backup, input_ports))
		return;

	/* open the config file */
	curfile.file = mame_fopen("default", 0, FILETYPE_CONFIG, 1);
	if (!curfile.file)
		goto error;
	curfile.filetype = FILE_TYPE_DEFAULT;

	/* save the XML */
	config_save_xml();
	mame_fclose(curfile.file);

error:
	cleanup_file();
}



/*************************************
 *
 *  Controller-specific data load
 *
 *************************************/

int config_load_controller(const char *name, struct InputPortDefinition *input_ports)
{
	int result;

	/* clear current file data to zero */
	memset(&curfile, 0, sizeof(curfile));

	/* open the config file */
	curfile.file = mame_fopen(NULL, name, FILETYPE_CTRLR, 0);
	if (!curfile.file)
		return 0;
	curfile.filetype = FILE_TYPE_CONTROLLER;

	/* load the XML */
	result = config_load_xml();
	mame_fclose(curfile.file);
	if (!result)
		return 0;

	/* return true only if we can apply the port data */
	result = apply_default_ports(NULL, input_ports);
	cleanup_file();
	return result;
}



/*************************************
 *
 *  Allocated data cleanup
 *
 *************************************/

static void cleanup_file(void)
{
	struct config_port *port, *next;

	/* free any allocated port data */
	for (port = curfile.data.port; port != NULL; port = next)
	{
		next = port->next;
		free(port);
	}
}



/*************************************
 *
 *  Helper to streamline defaults
 *
 *************************************/

INLINE input_code_t get_default_code(int type)
{
	switch (type)
	{
		case IPT_DIPSWITCH_NAME:
		case IPT_CATEGORY_NAME:
			return CODE_NONE;

		default:
			if (curfile.filetype != FILE_TYPE_GAME)
				return CODE_NONE;
			else
				return CODE_DEFAULT;
	}
	return CODE_NONE;
}




/*************************************
 *
 *  XML file load
 *
 *************************************/

static int config_load_xml(void)
{
	return 0;
}



/*************************************
 *
 *  XML file save
 *
 *************************************/
static int config_save_xml(void)
{
	return 1;
}



/*************************************
 *
 *  Identifies which port types are
 *  saved/loaded
 *
 *************************************/

static int save_this_port_type(int type)
{
	switch (type)
	{
		case IPT_UNUSED:
		case IPT_END:
		case IPT_PORT:
		case IPT_DIPSWITCH_SETTING:
		case IPT_CONFIG_SETTING:
		case IPT_CATEGORY_SETTING:
		case IPT_VBLANK:
		case IPT_UNKNOWN:
			return 0;
	}
	return 1;
}



/*************************************
 *
 *  Apply game-specific read port data
 *
 *************************************/

static int apply_ports(const struct InputPort *input_ports_default, struct InputPort *input_ports)
{
	struct config_port *port;
	const struct InputPort *cin;
	struct InputPort *in;

	/* compare the saved default settings against the current ones in the driver */
	for (port = curfile.data.port, cin = input_ports_default; cin->type != IPT_END; cin++)
		if (save_this_port_type(cin->type))
		{
			/* if we don't have any more ports, we're corrupt */
			if (port == NULL)
				return 0;

			/* if we don't match, return a corrupt error */
			if ((port->defvalue & port->mask) != (cin->default_value & cin->mask) ||
				port->mask != cin->mask || port->type != cin->type || port->player != cin->player)
				return 0;

			/* if the default sequence(s) don't match, return a corrupt error */
			if (!port_type_is_analog(port->type))
			{
				if (seq_cmp(&port->defseq[0], &cin->seq) != 0)
					return 0;
			}
			else
			{
				if (seq_cmp(&port->defseq[0], &cin->seq) != 0 ||
					seq_cmp(&port->defseq[1], &cin->analog.decseq) != 0 ||
					seq_cmp(&port->defseq[2], &cin->analog.incseq) != 0)
					return 0;
			}

			/* advance to the next port */
			port = port->next;
		}

	/* apply the current settings */
	for (port = curfile.data.port, in = input_ports; in->type != IPT_END; in++)
		if (save_this_port_type(in->type))
		{
			/* apply the current values */
			in->default_value = port->value;
			in->analog.delta = port->keydelta;
			in->analog.centerdelta = port->centerdelta;
			in->analog.sensitivity = port->sensitivity;
			in->analog.reverse = port->reverse;

			/* copy the sequence(s) */
			seq_copy(&in->seq, &port->newseq[0]);
			if (port_type_is_analog(port->type))
			{
				seq_copy(&in->analog.decseq, &port->newseq[1]);
				seq_copy(&in->analog.incseq, &port->newseq[2]);
			}

			/* advance to the next port */
			port = port->next;
		}

	return 1;
}



/*************************************
 *
 *  Apply default port data
 *
 *************************************/

static int apply_default_ports(const struct InputPortDefinition *input_ports_default_backup, struct InputPortDefinition *input_ports_default)
{
	struct config_port *port;
	int portnum, remapnum, seqnum;

	/* apply any remapping first */
	for (remapnum = 0; remapnum < __code_max; remapnum++)
		if (curfile.data.remap[remapnum] != remapnum)
			for (portnum = 0; input_ports_default[portnum].type != IPT_END; portnum++)
			{
				for (seqnum = 0; seqnum < SEQ_MAX; seqnum++)
					if (input_ports_default[portnum].defaultseq.code[seqnum] == remapnum)
						input_ports_default[portnum].defaultseq.code[seqnum] = curfile.data.remap[remapnum];

				if (port_type_is_analog(input_ports_default[portnum].type))
				{
					for (seqnum = 0; seqnum < SEQ_MAX; seqnum++)
						if (input_ports_default[portnum].defaultdecseq.code[seqnum] == remapnum)
							input_ports_default[portnum].defaultdecseq.code[seqnum] = curfile.data.remap[remapnum];
					for (seqnum = 0; seqnum < SEQ_MAX; seqnum++)
						if (input_ports_default[portnum].defaultincseq.code[seqnum] == remapnum)
							input_ports_default[portnum].defaultincseq.code[seqnum] = curfile.data.remap[remapnum];
				}
			}

	/* loop over the ports */
	for (port = curfile.data.port; port != NULL; port = port->next)
	{
		/* find a matching entry in the defaults */
		for (portnum = 0; input_ports_default[portnum].type != IPT_END; portnum++)
			if (input_ports_default[portnum].type == port->type && input_ports_default[portnum].player == port->player)
			{
				/* load stored settings only if the default hasn't changed or if the backup array is NULL */

				/* non-analog case */
				if (!port_type_is_analog(port->type))
				{
					if (input_ports_default_backup == NULL || seq_cmp(&input_ports_default_backup[portnum].defaultseq, &port->defseq[0]) == 0)
						seq_copy(&input_ports_default[portnum].defaultseq, &port->newseq[0]);
				}

				/* analog case */
				else
				{
					if (input_ports_default_backup == NULL ||
						(seq_cmp(&input_ports_default_backup[portnum].defaultseq, &port->defseq[0]) == 0 &&
						 seq_cmp(&input_ports_default_backup[portnum].defaultdecseq, &port->defseq[1]) == 0 &&
						 seq_cmp(&input_ports_default_backup[portnum].defaultincseq, &port->defseq[2]) == 0))
					{
						seq_copy(&input_ports_default[portnum].defaultseq, &port->newseq[0]);
						seq_copy(&input_ports_default[portnum].defaultdecseq, &port->newseq[1]);
						seq_copy(&input_ports_default[portnum].defaultincseq, &port->newseq[2]);
					}
				}
			}
	}

	return 1;
}



/*************************************
 *
 *  Apply counter/mixer data
 *
 *************************************/

static int apply_counters(void)
{
	int counternum;

	/* copy data from the saved config and reset the lastcount and lockout flags */
	for (counternum = 0; counternum < COIN_COUNTERS; counternum++)
		coin_count[counternum] = curfile.data.counters.coins[counternum];
	dispensed_tickets = curfile.data.counters.tickets;

	return 1;
}


static int apply_mixer(void)
{
	int num_vals = sound_get_user_gain_count();
	int mixernum;

	/* set the mixer gain on all channels */
	for (mixernum = 0; mixernum < num_vals; mixernum++)
		if (curfile.data.mixer.deflevel[mixernum] == sound_get_default_gain(mixernum))
			sound_set_user_gain(mixernum, curfile.data.mixer.newlevel[mixernum]);

	return 1;
}



/*************************************
 *
 *  Build game-specific port data
 *
 *************************************/

static int build_ports(const struct InputPort *input_ports_default, const struct InputPort *input_ports)
{
	struct config_port *porthead = NULL, *port, *next;
	int portnum;

	/* loop through the ports and allocate/fill in data structures along the way */
	for (portnum = 0; input_ports_default[portnum].type != IPT_END; portnum++)
		if (save_this_port_type(input_ports_default[portnum].type))
		{
			/* allocate memory for a new port */
			port = malloc(sizeof(*port));
			if (!port)
				goto error;
			memset(port, 0, sizeof(*port));

			/* add this port to our temporary list */
			port->next = porthead;
			porthead = port;

			/* fill in the data */
			port->type        = input_ports_default[portnum].type;
			port->player      = input_ports_default[portnum].player;
			port->mask        = input_ports_default[portnum].mask;
			port->defvalue    = input_ports_default[portnum].default_value;
			port->value       = input_ports[portnum].default_value;
			port->keydelta    = input_ports[portnum].analog.delta;
			port->centerdelta = input_ports[portnum].analog.centerdelta;
			port->sensitivity = input_ports[portnum].analog.sensitivity;
			port->reverse     = input_ports[portnum].analog.reverse;

			/* copy the sequences */
			seq_copy(&port->defseq[0], &input_ports_default[portnum].seq);
			seq_copy(&port->newseq[0], &input_ports[portnum].seq);
			if (port_type_is_analog(port->type))
			{
				seq_copy(&port->defseq[1], &input_ports_default[portnum].analog.decseq);
				seq_copy(&port->newseq[1], &input_ports[portnum].analog.decseq);
				seq_copy(&port->defseq[2], &input_ports_default[portnum].analog.incseq);
				seq_copy(&port->newseq[2], &input_ports[portnum].analog.incseq);
			}
		}

	/* now reverse the list and connect it to the config file */
	curfile.data.port = NULL;
	for (port = porthead; port != NULL; port = next)
	{
		next = port->next;
		port->next = curfile.data.port;
		curfile.data.port = port;
	}
	return 1;

error:
	/* free what we have and error */
	cleanup_file();
	return 0;
}



/*************************************
 *
 *  Build default port data
 *
 *************************************/

static int build_default_ports(const struct InputPortDefinition *input_ports_default_backup, const struct InputPortDefinition *input_ports_default)
{
	struct config_port *porthead = NULL, *port, *next;
	int portnum;

	/* loop through the ports and allocate/fill in data structures along the way */
	for (portnum = 0; input_ports_default[portnum].type != IPT_END; portnum++)
		if (save_this_port_type(input_ports_default[portnum].type))
		{
			/* allocate memory for a new port */
			port = malloc(sizeof(*port));
			if (!port)
				goto error;
			memset(port, 0, sizeof(*port));

			/* add this port to our temporary list */
			port->next = porthead;
			porthead = port;

			/* fill in the data */
			port->type        = input_ports_default[portnum].type;
			port->player      = input_ports_default[portnum].player;

			/* copy the sequences */
			seq_copy(&port->defseq[0], &input_ports_default_backup[portnum].defaultseq);
			seq_copy(&port->newseq[0], &input_ports_default[portnum].defaultseq);
			if (port_type_is_analog(port->type))
			{
				seq_copy(&port->defseq[1], &input_ports_default_backup[portnum].defaultdecseq);
				seq_copy(&port->newseq[1], &input_ports_default[portnum].defaultdecseq);
				seq_copy(&port->defseq[2], &input_ports_default_backup[portnum].defaultincseq);
				seq_copy(&port->newseq[2], &input_ports_default[portnum].defaultincseq);
			}
		}

	/* now reverse the list and connect it to the config file */
	curfile.data.port = NULL;
	for (port = porthead; port != NULL; port = next)
	{
		next = port->next;
		port->next = curfile.data.port;
		curfile.data.port = port;
	}
	return 1;

error:
	/* free what we have and error */
	cleanup_file();
	return 0;
}



/*************************************
 *
 *  Build counter/mixer data
 *
 *************************************/

static int build_counters(void)
{
	int counternum;

	/* copy out the coin/ticket counters for this machine */
	for (counternum = 0; counternum < COIN_COUNTERS; counternum++)
		curfile.data.counters.coins[counternum] = coin_count[counternum];
	curfile.data.counters.tickets = dispensed_tickets;
	return 1;
}


static int build_mixer(void)
{
	int num_vals = sound_get_user_gain_count();
	int mixernum;

	/* copy out the mixing levels */
	for (mixernum = 0; mixernum < num_vals; mixernum++)
	{
		if (mixernum < num_vals)
		{
			curfile.data.mixer.deflevel[mixernum] = sound_get_default_gain(mixernum);
			curfile.data.mixer.newlevel[mixernum] = sound_get_user_gain(mixernum);
		}
		else
			curfile.data.mixer.deflevel[mixernum] = curfile.data.mixer.newlevel[mixernum] = 0.0;
	}
	return 1;
}
