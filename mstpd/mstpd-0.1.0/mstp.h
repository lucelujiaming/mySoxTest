/*
 * mstp.h      State machines from IEEE 802.1Q-2005
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version
 *  2 of the License, or (at your option) any later version.
 *
 * Authors: Vitalii Demianets <dvitasgs@gmail.com>
 */

#ifndef MSTP_H
#define MSTP_H

#include <sys/types.h>
#include <stdlib.h>

#include "bridge_ctl.h"
#include "list.h"

/* #define HMAC_MDS_TEST_FUNCTIONS */

/* Useful macro for counting number of elements in array */
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

/*
 * assign() and cmp() macros that also do strict type-checking. See the
 * "unnecessary" pointer comparison.
 * NOTE: potential double-evaluation of the first argument in assign macro!
 *       It is the price for type-safety ;)
 */
#define assign(x, y) ({             \
    typeof(x) _assign1 = (x);       \
    typeof(y) _assign2 = (y);       \
    (void)(&_assign1 == &_assign2); \
    (x) = _assign2; })
#define _ncmp(x, y) ({          \
    typeof(x) _cmp1 = (x);      \
    typeof(y) _cmp2 = (y);      \
    (void)(&_cmp1 == &_cmp2);   \
    memcmp(&_cmp1, &_cmp2, sizeof(_cmp1)); })
#define cmp(x, _op, y) (_ncmp((x), (y)) _op 0)

/* 13.7, Table 13-1 */
#define HMAC_KEY    {0x13, 0xAC, 0x06, 0xA6, 0x2E, 0x47, 0xFD, 0x51, \
                     0xF9, 0x5D, 0x2B, 0xA2, 0x43, 0xCD, 0x03, 0x46}
extern void hmac_md5(unsigned char * text, int text_len, unsigned char * key,
                     int key_len, caddr_t digest);
#ifdef HMAC_MDS_TEST_FUNCTIONS
extern bool MD5TestSuite(void);
#endif /* HMAC_MDS_TEST_FUNCTIONS */

#define MAX_PORT_NUMBER 4095
#define MAX_VID         4094
#define MAX_FID         4095
#define MAX_MSTID       4094

/* MAX_xxx_MSTIS: CIST not counted */
#define MAX_STANDARD_MSTIS          64
#define MAX_IMPLEMENTATION_MSTIS    63

/* 13.37.1 */
#define MAX_PATH_COST   200000000u

typedef union
{
    __u64 u;
    struct
    {
        __be16 priority;
        __u8 mac_address[ETH_ALEN];
    } __attribute__((packed)) s;
} bridge_identifier_t;

typedef __be16 port_identifier_t;

/* These macros work well for both PortID and BridgeID */
#define GET_PRIORITY_FROM_IDENTIFIER(id)    (((__u8 *)(&(id)))[0] & 0xF0)
#define SET_PRIORITY_IN_IDENTIFIER(pri, id) do{ \
    __u8 *first_octet = (__u8 *)(&(id));        \
    *first_octet &= 0x0F;                       \
    *first_octet |= (pri) & 0xF0;               \
    }while(0)

#define CONFIGURATION_NAME_LEN   32
#define CONFIGURATION_DIGEST_LEN 16
typedef union
{
    __u8 a[1 + CONFIGURATION_NAME_LEN + 2 + CONFIGURATION_DIGEST_LEN];
    struct
    {
        __u8 selector; /* always 0 */
        __u8 configuration_name[CONFIGURATION_NAME_LEN];
        __be16 revision_level;
        __u8 configuration_digest[CONFIGURATION_DIGEST_LEN];
    } __attribute__((packed)) s;
} __attribute__((packed)) mst_configuration_identifier_t;

typedef struct
{
    bridge_identifier_t RRootID;
    __be32 IntRootPathCost;
    bridge_identifier_t DesignatedBridgeID;
    port_identifier_t DesignatedPortID;
    /* not used for MSTIs, only for CIST */
    bridge_identifier_t RootID;
    __be32 ExtRootPathCost;
} port_priority_vector_t;

typedef struct
{
    __u8 remainingHops;
    /* not used for MSTIs, only for CIST */
    __u8 Forward_Delay;
    __u8 Max_Age;
    __u8 Message_Age;
    __u8 Hello_Time;
} times_t;

typedef struct
{
    /* see bpduFlagOffset_t enum for offsets of flag bits */
    __u8 flags;
    bridge_identifier_t mstiRRootID;
    __be32 mstiIntRootPathCost;
    /* only bits 7..4, bits 3..0 are zero on Tx and ignored on Rx */
    __u8 bridgeIdentifierPriority;
    /* only bits 7..4, bits 3..0 are zero on Tx and ignored on Rx */
    // 端口标识符的可管理部件，也叫做端口优先级(14.8.2, Table 17-2)。
    __u8 portIdentifierPriority;
    __u8 remainingHops;
} __attribute__((packed)) msti_configuration_message_t;

typedef struct
{
    /* always zero for the Spanning Tree BPDUs */
    __be16 protocolIdentifier;
    /* protoSTP for the Config and TCN
     * protoRSTP for the RST
     * protoMSTP for the MST
     * (see protocol_version_t enum) */
    __u8 protocolVersion;
    /* values are defined in bpduType_t enum */
    __u8 bpduType;
    /* TCN BPDU ends here */
    /* see bpduFlagOffset_t enum for offsets of flag bits */
    __u8 flags;
    bridge_identifier_t cistRootID;
    __be32 cistExtRootPathCost;
    bridge_identifier_t cistRRootID;
    port_identifier_t cistPortID;
    __u8 MessageAge[2];
    __u8 MaxAge[2];
    __u8 HelloTime[2];
    __u8 ForwardDelay[2];
    /* Config BPDU ends here */
    __u8 version1_len; /* always zero */
    /* RST BPDU ends here */
    __be16 version3_len;
    mst_configuration_identifier_t mstConfigurationIdentifier;
    __be32 cistIntRootPathCost;
    bridge_identifier_t cistBridgeID;
    __u8 cistRemainingHops;
    msti_configuration_message_t mstConfiguration[MAX_STANDARD_MSTIS];
} __attribute__((packed)) bpdu_t;

#define TCN_BPDU_SIZE    offsetof(bpdu_t, flags)
#define CONFIG_BPDU_SIZE offsetof(bpdu_t, version1_len)
#define RST_BPDU_SIZE    offsetof(bpdu_t, version3_len)
#define MST_BPDU_SIZE_WO_MSTI_MSGS    offsetof(bpdu_t, mstConfiguration)
#define MST_BPDU_VER3LEN_WO_MSTI_MSGS (MST_BPDU_SIZE_WO_MSTI_MSGS \
                    - offsetof(bpdu_t, mstConfigurationIdentifier))

typedef enum
{
    OtherInfo,
    SuperiorDesignatedInfo,
    RepeatedDesignatedInfo,
    InferiorDesignatedInfo,
    InferiorRootAlternateInfo
} port_info_t;

typedef enum
{
    ioDisabled,
    ioMine,
    ioAged,
    ioReceived
} port_info_origin_t;

typedef enum
{
    roleDisabled,
    roleRoot,
    roleDesignated,
    roleAlternate,
    roleBackup,
    roleMaster
} port_role_t;

typedef enum
{
    encodedRoleMaster = 0,
    encodedRoleAlternateBackup = 1,
    encodedRoleRoot = 2,
    encodedRoleDesignated = 3
} port_encoded_role_t;

typedef enum
{
    protoSTP = 0,
    protoRSTP = 2,
    protoMSTP = 3
} protocol_version_t;

typedef enum
{
    bpduTypeConfig = 0,
    bpduTypeRST = 2,
    bpduTypeTCN = 128
} bpduType_t;

typedef enum
{
    offsetTc = 0,
    offsetProposal = 1,
    offsetRole = 2, /* actually, role is coded in two-bit field */
    offsetRole1 = 3, /* second bit of two-bit role field */
    offsetLearnig = 4,
    offsetForwarding = 5,
    offsetAgreement = 6,
    offsetTcAck = 7
/* in MSTI Configuration Message flags bit7 is used for Master flag */
#define offsetMaster    offsetTcAck
} bpduFlagOffset_t;

#define BPDU_FLAGS_ROLE_SET(role)   (((role) & 3) << offsetRole)
#define BPDU_FLAGS_ROLE_GET(flags)  (((flags) >> offsetRole) & 3)

typedef enum
{
    p2pAuto,
    p2pForceTrue,
    p2pForceFalse
}
admin_p2p_t;

/* 13.28  Port Receive state machine */
typedef enum
{
    PRSM_DISCARD,
    PRSM_RECEIVE
} PRSM_states_t;

/* 13.29  Port Protocol Migration state machine */
typedef enum
{
    PPMSM_CHECKING_RSTP,
    PPMSM_SELECTING_STP,
    PPMSM_SENSING
} PPMSM_states_t;

/* 13.30  Bridge Detection state machine */
typedef enum
{
    BDSM_EDGE,
    BDSM_NOT_EDGE
} BDSM_states_t;

/* 13.31  Port Transmit state machine */
typedef enum
{
    PTSM_TRANSMIT_INIT,
    PTSM_TRANSMIT_CONFIG,
    PTSM_TRANSMIT_TCN,
    PTSM_TRANSMIT_RSTP,
    PTSM_TRANSMIT_PERIODIC,
    PTSM_IDLE
} PTSM_states_t;

/* 13.32  Port Information state machine */
/* #define PISM_ENABLE_LOG */
typedef enum
{
    PISM_DISABLED,
    PISM_AGED,
    PISM_UPDATE,
    PISM_SUPERIOR_DESIGNATED,
    PISM_REPEATED_DESIGNATED,
    PISM_INFERIOR_DESIGNATED,
    PISM_NOT_DESIGNATED,
    PISM_OTHER,
    PISM_CURRENT,
    PISM_RECEIVE
} PISM_states_t;

/* 13.33  Port Role Selection state machine */
typedef enum
{
    PRSSM_INIT_TREE,
    PRSSM_ROLE_SELECTION
} PRSSM_states_t;

/* 13.34  Port Role Transitions state machine */
/* #define PRTSM_ENABLE_LOG */
typedef enum
{
 /* Disabled Port role transitions */
    PRTSM_INIT_PORT,
    PRTSM_DISABLE_PORT,
    PRTSM_DISABLED_PORT,
 /* MasterPort role transitions */
    PRTSM_MASTER_PROPOSED,
    PRTSM_MASTER_AGREED,
    PRTSM_MASTER_SYNCED,
    PRTSM_MASTER_RETIRED,
    PRTSM_MASTER_FORWARD,
    PRTSM_MASTER_LEARN,
    PRTSM_MASTER_DISCARD,
    PRTSM_MASTER_PORT,
 /* RootPort role transitions */
    PRTSM_ROOT_PROPOSED,
    PRTSM_ROOT_AGREED,
    PRTSM_ROOT_SYNCED,
    PRTSM_REROOT,
    PRTSM_ROOT_FORWARD,
    PRTSM_ROOT_LEARN,
    PRTSM_REROOTED,
    PRTSM_ROOT_PORT,
 /* DesignatedPort role transitions */
    PRTSM_DESIGNATED_PROPOSE,
    PRTSM_DESIGNATED_AGREED,
    PRTSM_DESIGNATED_SYNCED,
    PRTSM_DESIGNATED_RETIRED,
    PRTSM_DESIGNATED_FORWARD,
    PRTSM_DESIGNATED_LEARN,
    PRTSM_DESIGNATED_DISCARD,
    PRTSM_DESIGNATED_PORT,
 /* AlternatePort and BackupPort role transitions */
    PRTSM_BLOCK_PORT,
    PRTSM_BACKUP_PORT,
    PRTSM_ALTERNATE_PROPOSED,
    PRTSM_ALTERNATE_AGREED,
    PRTSM_ALTERNATE_PORT
} PRTSM_states_t;

/* 13.35  Port State Transition state machine */
typedef enum
{
    PSTSM_DISCARDING,
    PSTSM_LEARNING,
    PSTSM_FORWARDING
} PSTSM_states_t;

/* 13.36  Topology Change state machine */
typedef enum
{
    TCSM_INACTIVE,
    TCSM_LEARNING,
    TCSM_DETECTED,
    TCSM_NOTIFIED_TCN,
    TCSM_NOTIFIED_TC,
    TCSM_PROPAGATING,
    TCSM_ACKNOWLEDGED,
    TCSM_ACTIVE
} TCSM_states_t;

/*
 * Following standard-defined variables are not defined as variables.
 * Their functionality is implemented indirectly by other means:
 *  - BEGIN, tick, ageingTime.
 */

typedef struct
{
    struct list_head list; /* anchor in global list of bridges */

    /* List of all ports */
    struct list_head ports;
    /* List of all tree instances, first in list (trees.next) is CIST */
    struct list_head trees;
#define GET_CIST_TREE(br) list_entry((br)->trees.next, tree_t, bridge_list)

    bool bridgeEnabled;

    /* Per-bridge configuration parameters */
    mst_configuration_identifier_t MstConfigId; /* 13.24.b */
    // 桥的强制协议版本参数。取值0表示STP兼容模式，2表示普通模式。缺省为2。
    protocol_version_t ForceProtocolVersion; /* 13.22.e */
    __u8 MaxHops;             /* 13.22.o */
    // STP桥(17.4)使用的，把根端口和指定端口迁移到转发态的延时(Table 17-1)。
    __u8 Forward_Delay;       /* 13.22.f */
    // 根桥发送的最大信息生存时间(Table 17-1)。
    __u8 Max_Age;             /* 13.22.i */
    /* The 802.1Q-2005 (13.22.j) says that this parameter is substituted by
     * the per-port Hello Time, but we still need it for compatibility
     * with old STP implementations.
     */
    // 指定端口发送配置消息的周期(Table 17-1)。
    __u8 Hello_Time;
    unsigned int Transmit_Hold_Count; /* 13.22.g */
    // mdelayWhile和edgeDelayWhile定时器(17.17.4, 17.17.1)的初始值。
    // 根据本规范对于所有RSTP实现来说是固定的(Table 17-1)。
    unsigned int Migrate_Time;        /* 13.22.h */
    unsigned int Ageing_Time;  /* 8.8.3 */

    __u16 vid2fid[MAX_VID + 1];
    __be16 fid2mstid[MAX_FID + 1];

    /* not in standard */
    unsigned int uptime;

    sysdep_br_data_t sysdeps;
} bridge_t;

typedef struct
{
    struct list_head bridge_list; /* anchor in bridge's list of trees */
    bridge_t * bridge;
    __be16 MSTID; /* 0 == CIST */

    /* List of the per-port data structures for this tree instance */
    struct list_head ports;

    /* 13.23.(c,f,g) Per-bridge per-tree variables */
    // BridgeIdentifier: 分配给网桥的唯一标识符。包含网桥标识符优先级和网桥地址
    bridge_identifier_t BridgeIdentifier;
    // 根端口的端口标识符。这是根优先级向量的第五部分，参见17.6。
    port_identifier_t rootPortId;
    // 网桥的根优先级向量的第四部分，参见17.6。
    port_priority_vector_t rootPriority;

    /* 13.23.d This is totally calculated from BridgeIdentifier */
    // 网桥优先级向量
    port_priority_vector_t BridgePriority;

    /* 13.23.e Some waste of space here, as MSTIs only use
     * remainingHops member of the struct times_t,
     * but saves extra checks and improves readability */
    // BridgeTimes: 网桥定时器。包含四个部分：网桥转发延时、网桥Hello时间、
    //              最大老化时间的当前值，而消息生存时间为0。
    // rootTimes: 含网桥的操作定时器参数值，其值来自于portTimes或BridgeTimes。
    times_t BridgeTimes, rootTimes;

    /* 12.8.1.1.3.(b,c,d) */
    unsigned int time_since_topology_change;
    unsigned int topology_change_count;
    bool topology_change;
    char topology_change_port[IFNAMSIZ];
    char last_topology_change_port[IFNAMSIZ];

    /* State machines */
    PRSSM_states_t PRSSM_state;

} tree_t;

typedef struct
{
    struct list_head br_list; /* anchor in bridge's list of ports */
    bridge_t * bridge;
    __be16 port_number;

    /* List of all tree instances, first in list (trees.next) is CIST.
     * List is sorted by MSTID (by insertion procedure MSTP_IN_create_msti).
     */
    struct list_head trees;
#define GET_CIST_PTP_FROM_PORT(prt) \
    list_entry((prt)->trees.next, per_tree_port_t, port_list)

    /* 13.21.(a,b,c) Per-port timers */
    // edgeDelayWhile: 边缘延时定时器。在端口被识别为operEdgePort之前，
    //                 是尚未接收到BPDU的剩余时间。
    // helloWhen: Hello定时器。用来确保在每个HelloTime周期内，
    //            指定端口至少发送一个BPDU。
    // mdelayWhile: 迁迁移延时定时器。端口协议迁移状态机使用。
    //            本定时器初始值为MigrateTime(17.13.9)。
    unsigned int mdelayWhile, helloWhen, edgeDelayWhile;

    /* 13.24.(b,c,e,f,g,j,k,l,m,n,o,p,q,r,aw) Per-port variables */
    unsigned int txCount;
    bool operEdge, portEnabled, infoInternal, rcvdInternal;
    // 可以人工配置，以强制端口协议迁移状态机以MigrateTime (17.13.9)周期发送RST BPDU，
    // 以测试是否LAN上的所有STP桥(17.4)是否已经被删除，以及端口是否可以连续发送RSTP BPDU。
    bool mcheck, rcvdBpdu, rcvdRSTP, rcvdSTP, rcvdTcAck, rcvdTcn, sendRSTP;
    bool tcAck, newInfo, newInfoMsti;

    /* 6.4.3 */
    bool operPointToPointMAC;

    /* Per-port configuration parameters */
    bool restrictedRole, restrictedTcn; /* 13.24.(h,i) */
    // PortPathCost: 端口到根桥的路径开销(17.3.1, 17.5, 17.6)。
    __u32 ExternalPortPathCost; /* 13.22.p */
    __u32 AdminExternalPortPathCost; /* 0 = calculate from speed */
    admin_p2p_t AdminP2P; /* 6.4.3 */
    // 端口的AdminEdgePort参数(14.8.2)。
    bool AdminEdgePort; /* 13.22.k */
    // 桥的老化时间参数(7.9.2, Table 7-5)。
    bool AutoEdge; /* 13.22.m */
    bool BpduGuardPort;
    bool BpduGuardError;
    bool NetworkPort;
    bool BaInconsistent;
    bool dontTxmtBpdu;
    bool bpduFilterPort;

    unsigned int rapidAgeingWhile;
    unsigned int brAssuRcvdInfoWhile;

    /* State machines */
    PRSM_states_t PRSM_state;
    PPMSM_states_t PPMSM_state;
    BDSM_states_t BDSM_state;
    PTSM_states_t PTSM_state;

    /* Copy of the received BPDU */
    bpdu_t rcvdBpduData;
    int rcvdBpduNumOfMstis;

    bool deleted;

    sysdep_if_data_t sysdeps;
    unsigned int num_rx_bpdu_filtered;
    unsigned int num_rx_bpdu;
    unsigned int num_rx_tcn;
    unsigned int num_tx_bpdu;
    unsigned int num_tx_tcn;
    unsigned int num_trans_fwd;
    unsigned int num_trans_blk;
} port_t;

typedef struct
{
    struct list_head port_list; /* anchor in port's list of trees */
    struct list_head tree_list; /* anchor in tree's list of per-port data */
    port_t *port;
    tree_t *tree;
    __be16 MSTID; /* 0 == CIST */

    int state; /* BR_STATE_xxx */

    /* 13.21.(d,e,f,g,h) Per-port per-tree timers */
    // fdWhile: 转发延时定时器。用于延迟端口状态迁移，
    //          直到其它网桥接收到生成树信息为止。
    // rbWhile: 最近后备定时器。当端口是后备端口是，维持在初始值，HelloTime的两倍。
    // rcvdInfoWhile: 已接收信息定时器。在本端口接收的生成树信息老化之前，
    //                接收下一条配置消息的剩余时间。
    // rrWhile: 最近根定时器。
    // tcWhile: 拓扑变化定时器。当本定时器运行时发送TCN消息。
    unsigned int fdWhile, rrWhile, rbWhile, tcWhile, rcvdInfoWhile;

    /* 13.24.(s,t,u,v,w,x,y,z,aa,ab,ac,ad,ae,af,ag,ai,aj,ak,ap,as,at,au,av)
     * Per-port per-tree variables */
    // agree: 
    // 本状态机负责改变端口状态(7.4)。端口角色迁移状态机通过设置learn和forward变量
    // 来请求改变端口状态，端口状态迁移状态机在端口状态发生改变后，
    // 更新变量learning和forwarding。
    // agree (17.19.3).如果所有其它端口的synced变量值均为TRUE，则本变量为TRUE。
    //           发送Agreement标志位为TRUE的RST BPDU，且如果proposed为TRUE，
    //           则agree变量第一次为TRUE时，proposed变量被复位。
    // agreed (17.19.3).当端口接收到角色为根端口、替换端口或后备端口的RST BPDU，
    //           Agreement标志位为1，消息优先级低于或等于端口优先级，则本变量设置为TRUE。
    //           当agreed变量被设置为TRUE，指定端口就知道它的临桥已经被配置好了，
    //           不用延时，可以立刻进入转发态。
    bool agree, agreed, disputed, forward, forwarding, learn, learning;
    port_info_t rcvdInfo;
    // 用来指示端口的生成树信息的来源或状态。
    port_info_origin_t infoIs;
    // Figure 17-8显示了端口角色状态机把指定端口状态迁移到转发态所交换的消息，
    // 使用了如下的状态机变量：
    //    proposing (17.19.24).由没有处于转发态的指定端口设置，
    //              并且放在RST BPDU的Proposal位，发送给临桥的根端口或替换端口。
    //    proposed (17.19.23).当接收到具有指定端口角色的RST BPDU，且其Proposal位
    //              被设置时，本变量被设置为TRUE。如果变量agree为FALSE，本变量会
    //              导致桥上其它所有端口的sync变量设置为TRUE。
    //    sync (17.19.39).是否设置端口迁移到丢弃态，端口是边缘端口或synced变量
    //              为TRUE(表示已经失步)的例外。
    //    synced (17.19.40).当端口处于丢弃态或变量agree为TRUE时，设置本变量为TRUE。
    bool proposed, proposing, rcvdMsg, rcvdTc, reRoot, reselect, selected;
    // fdbFlush: 由拓扑改变状态机设置，用来指示过滤数据库删除该端口的所有数据。
    // 
    // 
    bool fdbFlush, tcProp, updtInfo, sync, synced;
    port_identifier_t portId;
    port_role_t role, selectedRole;

    /* 13.24.(al,an,aq) Some waste of space here, as MSTIs don't use
     * RootID and ExtRootPathCost members of the struct port_priority_vector_t,
     * but saves extra checks and improves readability */
    // designatedPriority: 端口的指定优先级向量的前四个部分。
    //                     指定优先级向量的第五个部分是portId。
    // msgPriority: 所接收BPDU内消息优先级向量的前四个部分
    port_priority_vector_t designatedPriority, msgPriority, portPriority;

    /* 13.24.(am,ao,ar) Some waste of space here, as MSTIs only use
     * remainingHops member of the struct times_t,
     * but saves extra checks and improves readability */
    // designatedTimes: 包含了一组定时器参数的值
    //                  (Message Age, Max Age, Forward Delay, and Hello Time)，
    //                  当updtInfo位TRUE时，用于更新端口时间。
    //                  由updtRolesTree()程序(17.21.25)负责更新。
    // msgTimes: 包含了所接收BPDU的定时器参数的值(Message Age, Max Age, 
    //           Forward Delay, and Hello Time)。
    times_t designatedTimes, msgTimes, portTimes;

    /* 13.24.(ax,ay) Per-port per-MSTI variables, not applicable to CIST */
    bool master, mastered;

    /* Per-port per-tree configuration parameters */
    // PortPathCost: 端口到根桥的路径开销(17.3.1, 17.5, 17.6)。
    __u32 InternalPortPathCost; /* 13.22.q */
    __u32 AdminInternalPortPathCost; /* 0 = calculate from speed */

    /* not in standard, used for calculation of port uptime */
    unsigned int start_time;

    /* State machines */
    PISM_states_t PISM_state;
    PRTSM_states_t PRTSM_state;
    PSTSM_states_t PSTSM_state;
    TCSM_states_t TCSM_state;

    /* Auxiliary flag, helps preventing infinite recursion */
    bool calledFromFlushRoutine;

    /* Pointer to the corresponding MSTI Configuration Message
     * in the port->rcvdBpduData */
    msti_configuration_message_t *rcvdMstiConfig;
} per_tree_port_t;

/* External events (inputs) */
bool MSTP_IN_bridge_create(bridge_t *br, __u8 *macaddr);
bool MSTP_IN_port_create_and_add_tail(port_t *prt, __u16 portno);
void MSTP_IN_delete_port(port_t *prt);
void MSTP_IN_delete_bridge(bridge_t *br);
void MSTP_IN_set_bridge_address(bridge_t *br, __u8 *macaddr);
void MSTP_IN_set_bridge_enable(bridge_t *br, bool up);
void MSTP_IN_set_port_enable(port_t *prt, bool up, int speed, int duplex);
void MSTP_IN_one_second(bridge_t *br);
void MSTP_IN_all_fids_flushed(per_tree_port_t *ptp);
void MSTP_IN_rx_bpdu(port_t *prt, bpdu_t *bpdu, int size);

bool MSTP_IN_set_vid2fid(bridge_t *br, __u16 vid, __u16 fid);
bool MSTP_IN_set_all_vids2fids(bridge_t *br, __u16 *vids2fids);
bool MSTP_IN_set_fid2mstid(bridge_t *br, __u16 fid, __u16 mstid);
bool MSTP_IN_set_all_fids2mstids(bridge_t *br, __u16 *fids2mstids);
bool MSTP_IN_get_mstilist(bridge_t *br, int *num_mstis, __u16 *mstids);
bool MSTP_IN_create_msti(bridge_t *br, __u16 mstid);
bool MSTP_IN_delete_msti(bridge_t *br, __u16 mstid);
void MSTP_IN_set_mst_config_id(bridge_t *br, __u16 revision, __u8 *name);

/* External actions (outputs) */
void MSTP_OUT_set_state(per_tree_port_t *ptp, int new_state);
void MSTP_OUT_flush_all_fids(per_tree_port_t *ptp);
void MSTP_OUT_set_ageing_time(port_t *prt, unsigned int ageingTime);
void MSTP_OUT_tx_bpdu(port_t *prt, bpdu_t *bpdu, int size);
void MSTP_OUT_shutdown_port(port_t *prt);

/* Structures for communicating with user */
 /* 12.8.1.1 Read CIST Bridge Protocol Parameters */
typedef struct
{
    bridge_identifier_t bridge_id;
    unsigned int time_since_topology_change;
    unsigned int topology_change_count;
    bool topology_change;
    char topology_change_port[IFNAMSIZ];
    char last_topology_change_port[IFNAMSIZ];
    bridge_identifier_t designated_root;
    unsigned int root_path_cost;
    port_identifier_t root_port_id;
    __u8 root_max_age;
    __u8 root_forward_delay;
    __u8 bridge_max_age;
    __u8 bridge_forward_delay;
    unsigned int tx_hold_count;
    protocol_version_t protocol_version;
    bridge_identifier_t regional_root;
    unsigned int internal_path_cost;
    bool enabled; /* not in standard */
    // 桥的老化时间参数(7.9.2, Table 7-5)。
    unsigned int Ageing_Time;
    __u8 max_hops;
    __u8 bridge_hello_time;
} CIST_BridgeStatus;

void MSTP_IN_get_cist_bridge_status(bridge_t *br, CIST_BridgeStatus *status);

 /* 12.8.1.2 Read MSTI Bridge Protocol Parameters */
typedef struct
{
    bridge_identifier_t bridge_id;
    unsigned int time_since_topology_change;
    unsigned int topology_change_count;
    bool topology_change;
    char topology_change_port[IFNAMSIZ];
    char last_topology_change_port[IFNAMSIZ];
    bridge_identifier_t regional_root;
    unsigned int internal_path_cost;
    port_identifier_t root_port_id;
} MSTI_BridgeStatus;

void MSTP_IN_get_msti_bridge_status(tree_t *tree, MSTI_BridgeStatus *status);

/* 12.8.1.3 Set CIST Bridge Protocol Parameters */
typedef struct
{
    __u8 bridge_max_age;
    bool set_bridge_max_age;

    __u8 bridge_forward_delay;
    bool set_bridge_forward_delay;

    /* Superseded by MSTP_IN_set_msti_bridge_config for the CIST.
     * __u8 bridge_priority;
     * bool set_bridge_priority; */

    protocol_version_t protocol_version;
    bool set_protocol_version;

    unsigned int tx_hold_count;
    bool set_tx_hold_count;

    __u8 max_hops;
    bool set_max_hops;

    __u8 bridge_hello_time;
    bool set_bridge_hello_time;

    unsigned int bridge_ageing_time;
    bool set_bridge_ageing_time;
} CIST_BridgeConfig;

int MSTP_IN_set_cist_bridge_config(bridge_t *br, CIST_BridgeConfig *cfg);

/* 12.8.1.4 Set MSTI Bridge Protocol Parameters */
    /* No need in special structure for single parameter Bridge Priority */

int MSTP_IN_set_msti_bridge_config(tree_t *tree, __u8 bridge_priority);

/* 12.8.2.1 Read CIST Port Parameters */
typedef struct
{
    unsigned int uptime;
    int state; /* BR_STATE_xxx */
    port_identifier_t port_id;
    __u32 admin_external_port_path_cost; /* not in standard. 0 = auto */
    __u32 external_port_path_cost;
    bridge_identifier_t designated_root; /* from portPriority */
    __u32 designated_external_cost; /* from portPriority */
    bridge_identifier_t designated_bridge; /* from portPriority */
    port_identifier_t designated_port; /* from portPriority */
    bool tc_ack; /* tcAck */
    __u8 port_hello_time; /* from portTimes */
    bool admin_edge_port;
    bool auto_edge_port; /* not in standard */
    bool oper_edge_port;
    /* 802.1Q-2005 wants here MAC_Enabled & MAC_Operational. We don't know
     * neither of these. Return portEnabled and feel happy. */
    bool enabled;
    admin_p2p_t admin_p2p;
    bool oper_p2p;
    bool restricted_role;
    bool restricted_tcn;
    port_role_t role;
    bool disputed;
    bridge_identifier_t designated_regional_root; /* from portPriority */
    __u32 designated_internal_cost; /* from portPriority */
    __u32 admin_internal_port_path_cost; /* not in standard. 0 = auto */
    __u32 internal_port_path_cost; /* not in standard */
    bool bpdu_guard_port;
    bool bpdu_guard_error;
    bool bpdu_filter_port;
    bool network_port;
    bool ba_inconsistent;
    unsigned int num_rx_bpdu_filtered;
    unsigned int num_rx_bpdu;
    unsigned int num_rx_tcn;
    unsigned int num_tx_bpdu;
    unsigned int num_tx_tcn;
    unsigned int num_trans_fwd;
    unsigned int num_trans_blk;
    bool rcvdBpdu;
    bool rcvdRSTP;
    bool rcvdSTP;
    bool rcvdTcAck;
    bool rcvdTcn;
    bool sendRSTP;
} CIST_PortStatus;

void MSTP_IN_get_cist_port_status(port_t *prt, CIST_PortStatus *status);

/* 12.8.2.2 Read MSTI Port Parameters */
typedef struct
{
    unsigned int uptime;
    int state; /* BR_STATE_xxx */
    port_identifier_t port_id;
    __u32 admin_internal_port_path_cost; /* not in standard. 0 = auto */
    __u32 internal_port_path_cost;
    bridge_identifier_t designated_regional_root; /* from portPriority */
    __u32 designated_internal_cost; /* from portPriority */
    bridge_identifier_t designated_bridge; /* from portPriority */
    port_identifier_t designated_port; /* from portPriority */
    port_role_t role;
    bool disputed;
} MSTI_PortStatus;

void MSTP_IN_get_msti_port_status(per_tree_port_t *ptp,
                                  MSTI_PortStatus *status);

/* 12.8.2.3 Set CIST port parameters */
typedef struct
{
    __u32 admin_external_port_path_cost; /* not in standard. 0 = auto */
    bool set_admin_external_port_path_cost;

    /* Superseded by MSTP_IN_set_msti_port_config for the CIST.
     * __u32 admin_internal_port_path_cost;
     * bool set_admin_internal_port_path_cost;
     *
     * __u8 port_priority;
     * bool set_port_priority;
     */

    bool admin_edge_port;
    bool set_admin_edge_port;

    bool auto_edge_port; /* not in standard */
    bool set_auto_edge_port;

    admin_p2p_t admin_p2p;
    bool set_admin_p2p;

    bool restricted_role;
    bool set_restricted_role;

    bool restricted_tcn;
    bool set_restricted_tcn;

    bool bpdu_guard_port;
    bool set_bpdu_guard_port;

    bool network_port;
    bool set_network_port;

    bool dont_txmt;
    bool set_dont_txmt;

    bool bpdu_filter_port;
    bool set_bpdu_filter_port;
} CIST_PortConfig;

int MSTP_IN_set_cist_port_config(port_t *prt, CIST_PortConfig *cfg);

/* 12.8.2.4 Set MSTI port parameters */
typedef struct
{
    __u32 admin_internal_port_path_cost; /* 0 = auto */
    bool set_admin_internal_port_path_cost;

    __u8 port_priority;
    bool set_port_priority;
} MSTI_PortConfig;

int MSTP_IN_set_msti_port_config(per_tree_port_t *ptp, MSTI_PortConfig *cfg);

/* 12.8.2.5 Force BPDU Migration Check */
int MSTP_IN_port_mcheck(port_t *prt);

#endif /* MSTP_H */
