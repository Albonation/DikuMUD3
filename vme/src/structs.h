/*
 $Author: All $
 $RCSfile: structs.h,v $
 $Date: 2005/06/28 20:17:48 $
 $Revision: 2.9 $
 */
#pragma once

#include "char_point_data.h"
#include "color.h"
#include "combat.h"
#include "destruct.h"
#include "event.h"
#include "extra.h"
#include "hookmud.h"
#include "namelist.h"
#include "pc_account_data.h"
#include "pc_time_data.h"
#include "protocol.h"
#include "queue.h"
#include "room_direction_data.h"

#include <vme.h>

#include <cstring>
#include <forward_list>
#include <map>
#include <ostream>
#include <vector>

#ifndef MPLEX_COMPILE
    #include <boost/graph/adjacency_list.hpp>
    #include <boost/graph/graph_traits.hpp>
#endif

#define FI_MAX_ZONENAME 30 /* Max length of any zone-name    */
#define FI_MAX_UNITNAME 15 /* Max length of any unit-name    */

#define PC_MAX_PASSWORD 13 /* Max length of any pc-password  */
#define PC_MAX_NAME 15     /* 14 Characters + Null           */

#define MESS_ATTACKER 1
#define MESS_VICTIM 2
#define MESS_ROOM 3

/* For use in spec_assign.c's g_unit_function_array[] */
#define SD_NEVER 0 /* Never save this function           */
#define SD_NULL 1  /* Ignore fptr->data (save as 0 ptr)  */
#define SD_ASCII 2 /* If pointer, then it's ascii char * */

class char_follow_type;
class descriptor_data;
class dilprg;
class diltemplate;
class file_index_type;
class unit_affected_type;
class unit_data;
class unit_dil_affected_type;
class unit_fptr;
class zone_reset_cmd;

/* ----------------- DATABASE STRUCTURES ----------------------- */

/* ----------------- OTHER STRUCTURES ----------------------- */

/* --------------------- DESCRIPTOR STRUCTURES -------------------- */

/* ----------------- UNIT GENERAL STRUCTURES ----------------------- */

class unit_data : public basedestruct
{
public:
    unit_data();
    explicit unit_data(ubit8 unit_type)
        : unit_data()
    {
        status = unit_type;
    }
    virtual ~unit_data();
    unit_data *copy();
    void set_fi(file_index_type *f);

    cNamelist &getNames() { return names; }
    const cNamelist &getNames() const { return names; }

    unit_fptr *getFunctionPointer() { return func; }
    void setFunctionPointer(unit_fptr *value) { func = value; }

    /// HERE FOR PTR

    ubit32 getManipulate() const { return manipulate; }
    ubit32 *getManipulatePtr() { return &manipulate; }
    void setAllManipulateFlags(ubit32 value) { manipulate = value; }
    void readManipulateFrom(CByteBuffer &buf, ubit8 unit_version, int &errors)
    {
        if (unit_version < 46)
        {
            manipulate = buf.ReadU16(&errors);
        }
        else
        {
            manipulate = buf.ReadU32(&errors);
        }
    }
    void setManipulateFlag(ubit32 value) { manipulate |= value; }

    ubit16 getUnitFlags() const { return flags; }
    ubit16 *getUnitFlagsPtr() { return &flags; }
    void setAllUnitFlags(ubit16 value) { flags = value; }
    void setUnitFlag(ubit16 value) { flags |= value; }
    void removeUnitFlag(ubit16 value) { flags &= ~value; }
    void readFlagsFrom(CByteBuffer &buf, int &errors) { flags = buf.ReadU16(&errors); }

    sbit32 getBaseWeight() const { return base_weight; }
    void setBaseWeight(sbit32 value) { base_weight = value; }
    void readBaseWeightFrom(CByteBuffer &buf, int &errors) { base_weight = buf.ReadS16(&errors); }

    sbit32 getWeight() const { return weight; }
    void reduceWeightBy(sbit16 value) { weight -= value; }
    void increaseWeightBy(sbit16 value) { weight += value; }
    void setWeight(sbit32 value) { weight = value; }
    void readWeightFrom(CByteBuffer &buf, int &errors) { weight = buf.ReadS16(&errors); }

    sbit16 getCapacity() const { return capacity; }
    sbit16 *getCapacityPtr() { return &capacity; }
    void readCapacityFrom(CByteBuffer &buf, int &errors) { capacity = buf.ReadS16(&errors); }
    void setCapacity(sbit16 value) { capacity = value; }

    ubit16 getSize() const { return size; }
    ubit16 *getSizePtr() { return &size; }
    void setSize(sbit16 value) { size = value; }
    void readSizeFrom(CByteBuffer &buf, int &errors) { size = buf.ReadU16(&errors); }
    void increaseSizeBy(sbit16 value) { size += value; }

    ubit8 getUnitType() const { return status; }

    ubit8 getOpenFlags() const { return open_flags; }
    ubit8 *getOpenFlagsPtr() { return &open_flags; }
    void setAllOpenFlags(ubit8 value) { open_flags = value; }
    void readOpenFlagsFrom(CByteBuffer &buf, int &errors) { open_flags = buf.ReadU8(&errors); }
    void setOpenFlag(ubit8 value) { open_flags |= value; }

    ubit8 getOpenDifficulty() const { return open_diff; }
    ubit8 *getOpenDifficultyPtr() { return &open_diff; }
    void setOpenDifficulty(ubit8 value) { open_diff = value; }
    void readOpenDifficultyFrom(CByteBuffer &buf, ubit8 unit_version, int &errors)
    {
        if (unit_version >= 71)
        {
            open_diff = buf.ReadU8(&errors);
        }
    }

    sbit16 getNumberOfActiveLightSources() const { return light; }
    void increaseNumberOfActiveLightSourcesBy(sbit16 value) { light += value; }
    void decreaseNumberOfActiveLightSourcesBy(sbit16 value) { light -= value; }
    void setNumberOfActiveLightSources(sbit16 value) { light = value; }
    void readNumberOfActiveLightSourcesFrom(CByteBuffer &buf, int &errors) { light = buf.ReadS8(&errors); }

    sbit16 getLightOutput() const { return bright; }
    void setLightOutput(sbit16 value) { bright = value; }
    void increaseLightOutputBy(sbit16 value) { bright += value; }
    void decreaseLightOutputBy(sbit16 value) { bright -= value; }
    void readLightOutputFrom(CByteBuffer &buf, int &errors) { bright = buf.ReadS8(&errors); }

    sbit16 getTransparentLightOutput() const { return illum; }
    void setTransparentLightOutput(sbit16 value) { illum = value; }
    void increaseTransparentLightOutputBy(sbit16 value) { illum += value; }
    void decreaseTransparentLightOutputBy(sbit16 value) { illum -= value; }
    void readTransparentLightOutputFrom(CByteBuffer &buf, int &errors) { illum = buf.ReadS8(&errors); }

    sbit8 getNumberOfCharactersInsideUnit() const { return chars; }
    void readNumberOfCharactersInsideUnitFrom(CByteBuffer &buf, int &errors) { chars = buf.ReadU8(&errors); }
    void decrementNumberOfCharactersInsideUnit() { --chars; }
    void incrementNumberOfCharactersInsideUnit() { ++chars; }
    void setNumberOfCharactersInsideUnit(sbit8 value) { chars = value; }

    ubit8 getLevelOfWizardInvisibility() const { return minv; }
    ubit8 *getLevelOfWizardInvisibilityPtr() { return &minv; }
    void readLevelOfWizardInvisibilityFrom(CByteBuffer &buf, int &errors) { minv = buf.ReadU8(&errors); }
    void setLevelOfWizardInvisibility(ubit8 value) { minv = value; }

    sbit32 getMaximumHitpoints() const { return max_hp; }
    sbit32 *getMaximumHitpointsPtr() { return &max_hp; }
    void setMaximumHitpoints(sbit32 value) { max_hp = value; }
    void readMaximumHitpointsFrom(CByteBuffer &buf, int &errors) { max_hp = buf.ReadS32(&errors); }

    sbit32 getCurrentHitpoints() const { return hp; }
    sbit32 *getCurrentHitpointsPtr() { return &hp; }
    void setCurrentHitpoints(sbit32 value) { hp = value; }
    void increaseCurrentHitpointsBy(sbit32 value) { hp += value; }
    void decreaseCurrentHitpointsBy(sbit32 value) { hp -= value; }
    void readCurrentHitpointsFrom(CByteBuffer &buf, ubit8 unit_version, int &errors)
    {
        hp = buf.ReadS32(&errors);
        if (unit_version <= 54)
        {
            if (max_hp <= 0)
            {
                hp = 1000;
                max_hp = 1000;
            }
        }
    }

    sbit16 getAlignment() const { return alignment; }
    sbit16 *getAlignmentPtr() { return &alignment; }
    void increaseAlignmentBy(sbit16 value) { alignment += value; }
    void decreaseAlignmentBy(sbit16 value) { alignment -= value; }
    void setAlignment(sbit16 value) { alignment = value; }
    void readAlignmentFrom(CByteBuffer &buf, int &errors) { alignment = buf.ReadS16(&errors); }

    const std::string &getTitle() const { return title; }
    std::string *getTitlePtr() { return &title; }
    void setTitle(std::string value) { title = std::move(value); }

    const std::string &getDescriptionOfOutside() const { return out_descr; }
    std::string *getDescriptionOfOutsidePtr() { return &out_descr; }
    void setDescriptionOfOutside(std::string value) { out_descr = std::move(value); }

    const std::string &getDescriptionOfInside() const { return in_descr; }
    std::string *getDescriptionOfInsidePtr() { return &in_descr; }
    void setDescriptionOfInside(std::string value) { in_descr = std::move(value); }

    const extra_list &getExtraList() const { return extra; }
    extra_list &getExtraList() { return extra; }

    int destruct_classindex();
    std::string json();

private:
    cNamelist names;                            // Name Keyword list for get, enter, etc.
    unit_fptr *func{nullptr};                   // Function pointer type
    unit_dil_affected_type *dilaffect{nullptr}; //

public:
    unit_affected_type *affected;

    file_index_type *fi; /* Unit file-index                               */

    char *key; /* Pointer to fileindex to Unit which is the key */

    unit_data *outside; /* Pointer out of the unit, ei. from an object   */
    /* out to the char carrying it                   */
    unit_data *inside; /* Linked list of chars,rooms & objs             */

    unit_data /* For next unit in 'inside' linked list         */
        *next;

    unit_data /* global l-list of objects, chars & rooms       */
        *gnext,
        *gprevious;

private:
    ubit32 manipulate{0};  // WEAR_XXX macros
    ubit16 flags{0};       // Invisible, can_bury, burried...
    sbit32 base_weight{0}; // The "empty" weight of a room/char/obj (lbs)
    sbit32 weight{0};      // Current weight of a room/obj/char
    sbit16 capacity{0};    // Capacity of obj/char/room, -1 => any
    ubit16 size{0};        // (cm) MOBs height, weapons size, ropes length
    ubit8 status{0};       // IS_ROOM, IS_OBJ, IS_PC, IS_NPC
    ubit8 open_flags{0};   // In general OPEN will mean can "enter"?
    ubit8 open_diff{0};    // Open difficulty
    sbit16 light{0};       // Number of active light sources in unit
    sbit16 bright{0};      // How much the unit shines
    sbit16 illum{0};       // how much bright is by transparency
    ubit8 chars{0};        // How many chars is inside the unit
    ubit8 minv{0};         // Level of wizard invisible
    sbit32 max_hp{0};      // The maximum number of hitpoints
    sbit32 hp{0};          // The actual amount of hitpoints left
    sbit16 alignment{0};   // +-1000 for alignments
    std::string title;     // Room title, Char title, Obj "the barrel", NPC "the Beastly Fido"
    std::string out_descr; // The outside description of a unit
    std::string in_descr;  // The inside description of a unit
    extra_list extra;      // All the look 'at' stuff
};

/* ----------------- ROOM SPECIFIC STRUCTURES ----------------------- */

class room_data : public unit_data
{
public:
    room_data();
    ~room_data();

    room_direction_data *dir_option[MAX_EXIT + 1]; // Why 11? Why not MAX_EXIT+1?

    ubit8 flags;         /* Room flags                              */
    ubit8 movement_type; /* The type of movement (city, hills etc.) */
    ubit8 resistance;    /* Magic resistance of the room            */

    sbit16 mapx, mapy; /* Graphical map coordinates */

    int sc;  /*strong component, used for shortest path */
    int num; /*room number, used for shortest path */
#ifndef MPLEX_COMPILE
    enum edge_dir_t
    {
        edge_dir = 101
    };

    typedef boost::adjacency_list<boost::vecS,
                                  boost::vecS,
                                  boost::directedS,
                                  boost::no_property,
                                  boost::property<boost::edge_weight_t, int, boost::property<edge_dir_t, int>>>
        graph_t;

    typedef boost::graph_traits<graph_t>::vertex_descriptor vertex_descriptor;
    std::vector<vertex_descriptor> path;
    std::vector<vertex_descriptor> distance;
    int waiting_dijkstra;
#endif
};

/* ------------------ OBJ SPECIFIC STRUCTURES ----------------------- */

class obj_data : public unit_data
{
public:
    obj_data();
    ~obj_data();

    sbit32 value[5];     /* Values of the item (see list)       */
    ubit32 cost;         /* Value when sold (gp.)               */
    ubit32 cost_per_day; /* Cost to keep pr. real day           */

    ubit8 flags;      /* Various special object flags        */
    ubit8 type;       /* Type of item (ITEM_XXX)             */
    ubit8 equip_pos;  /* 0 or position of item in equipment  */
    ubit8 resistance; /* Magic resistance                    */
};

/* ----------------- CHAR SPECIFIC STRUCTURES ----------------------- */

class char_data : public unit_data
{
public:
    char_data(ubit8 unit_type);
    virtual ~char_data();

    descriptor_data *descriptor;
    cCombat *Combat;
    unit_data *master;    /* Must be a char */
    unit_data *last_room; /* Last location of character */
    char_point_data points;

    char_follow_type *followers;

    char *last_attacker; /* Last attacker of character */
    char *money;         /*  Money transfer from db-files. */

    ubit8 last_attacker_type; /* Last attacker type of character */
};

/* ------------------  PC SPECIFIC STRUCTURES ------------------------ */

class pc_data : public char_data
{
public:
    pc_data();
    ~pc_data();

    void gstate_tomenu(dilprg *pdontstop);
    void gstate_togame(dilprg *pdontstart);

    void disconnect_game();
    void connect_game();
    void reconnect_game(descriptor_data *d);

    terminal_setup_type setup;

    pc_time_data m_time{};     /* PCs time info  */
    pc_account_data account{}; /* Accounting     */

    char *guild;     // Player's current default guild (guilds in .info)
    char *bank;      /* How much money in bank?                 */
    char *hometown;  /* PCs Hometown (symbolic reference)       */
    char *promptstr; /* A PC's Prompt                           */

    extra_list info;  /* For saving Admin information             */
    extra_list quest; /* For saving QUEST information            */

    sbit8 profession; // The player's chosen profession, -1 means unknown
    ubit16 vlvl;      /* Virtual Level for player                */

    sbit32 id;             /* Unique identifier for each player (-1 guest) */
    sbit32 skill_points;   /* No of practice points left              */
    sbit32 ability_points; /* No of practice points left              */

    ubit16 flags;          /* flags for PC setup (brief, noshout...)  */
    ubit16 nr_of_crimes;   /* Number of crimes committed              */
    ubit16 crack_attempts; /* Number of wrong passwords entered       */
    ubit16 lifespan;       /* How many year to live....               */

    sbit16 spells[SPL_TREE_MAX];   /* The spells learned                  */
    ubit8 spell_lvl[SPL_TREE_MAX]; /* Practiced within that level         */

    sbit16 skills[SKI_TREE_MAX];   /* The skills learned                  */
    ubit8 skill_lvl[SKI_TREE_MAX]; /* The skills practiced within level   */

    sbit16 weapons[WPN_TREE_MAX];   /* The weapons learned                 */
    ubit8 weapon_lvl[WPN_TREE_MAX]; /* The weapons learned                  */

    ubit8 ability_lvl[ABIL_TREE_MAX]; /* The abilities learned                  */

    sbit8 conditions[3]; /* Drunk full etc.                     */
    ubit8 nAccessLevel;  /* Access Level for BBS use            */

    char pwd[PC_MAX_PASSWORD];  /* Needed when loaded w/o descriptor   */
    char filename[PC_MAX_NAME]; /* The name on disk...                 */
    ubit32 lasthosts[5];        /* last 5 different IPs                */
    color_type color;           /* Players default colors              */
};

/* ------------------ NPC SPECIFIC STRUCTURES ----------------------- */

class npc_data : public char_data
{
public:
    npc_data();
    ~npc_data();

    std::array<sbit16, WPN_GROUP_MAX> weapons;
    std::array<sbit16, SPL_GROUP_MAX> spells;

    ubit8 default_pos; /* Default position for NPC               */
    ubit8 flags;       /* flags for NPC behavior                 */
};

/* ----------------- Destructed decalrations ----------------------- */

#define DR_UNIT 0
#define DR_AFFECT 1
#define DR_FUNC 2

unit_data *new_unit_data(ubit8 type);

extern int g_world_nochars;
extern int g_world_nonpc;
extern int g_world_noobjects;
extern int g_world_norooms;
extern int g_world_nopc;
extern int g_world_nozones;
