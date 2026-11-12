// game/TeamAI_unk.cpp: AI of a team of characters (TeamAI_unk, include/game/TeamAIManager.h).
// File name invented. The members of a team are characters: TeamChara_unk is the view of
// include/game/TeamAIManager.h, used here as a CharaTask.
#include "game/TeamAIManager.h"
#include "game/BattleTask.h"
#include "game/GameTask.h"
#include "game/CharaTask.h"
#include "game/AiTarget.h"
#include "game/PlayerManager.h"
#include "std/tree.h"
#include "std/msl_common.h"

extern "C" GameTask* lbl_8087F430; // inferred: the field

extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" void PSMTXRotRad(Mtx m, char axis, float rad);
extern "C" int rand();
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);

extern "C" BattleTask* lbl_8087F048; // inferred: battle task
extern "C" PlayerManager* lbl_8087F8A0; // inferred: player manager
extern "C" int sprintf(char* buf, const char* fmt, ...);
// inferred: debug message list (as in gmk/RandomMerchant.cpp)
struct DebugMessages_unk {
    void fn_800697D8(const char* message);
};
extern "C" DebugMessages_unk* lbl_8087EEB8;

// inferred: the member as a character (TeamChara_unk is a partial view)
inline CharaTask* Chara_unk(TeamChara_unk* chara) {
    return (CharaTask*)chara;
}

// inferred: std::map<float, int> of MSL (<map> is not available here), laid out by hand as the trees of
// include/game/GameEventScript.h: Move_unk sorts the members and the points by their position along a
// line. The value is a point id or a member (CharaTask*) stored as an int: both maps share the weak copies
// of the tree (0x805B3120 insert, 0x805B3258 destroy, 0x805B33B8 insert position). Names invented.
struct TeamDistPair_unk {
    TeamDistPair_unk(const float& key, const int& value) : first(key), second(value) {}
    float first;
    int second;
};
struct TeamDistNode_unk {
    typedef TeamDistPair_unk Value;
    TeamDistNode_unk* mLeft;
    TeamDistNode_unk* mRight;
    unsigned int mParent;
    TeamDistPair_unk mValue;

    // inferred: MSL tree iterator increment (inlined)
    TeamDistNode_unk* Next_unk() {
        TeamDistNode_unk* node = this;
        if (node->mRight != 0) {
            node = node->mRight;
            while (node->mLeft != 0) {
                node = node->mLeft;
            }
            return node;
        }
        TeamDistNode_unk* parent = (TeamDistNode_unk*)(node->mParent & ~1);
        while (node != parent->mLeft) {
            node = parent;
            parent = (TeamDistNode_unk*)(node->mParent & ~1);
        }
        return parent;
    }
};
// inferred: result of map::insert (the bool is a constant temporary in .sdata)
struct TeamDistResult_unk {
    TeamDistResult_unk(TeamDistNode_unk* const& node, const bool& inserted) : first(node), second(inserted) {}
    TeamDistNode_unk* first;
    bool second;
};
template <class Node>
struct TeamDistTreeData_unk {
    TeamDistTreeData_unk() : mSize(0), mRoot(0), mFirst((Node*)&mRoot) {}
    ~TeamDistTreeData_unk() {
        if (mRoot != 0) {
            Destroy_unk(mRoot);
        }
    }
    void Destroy_unk(Node* node) {
        if (node->mLeft != 0) {
            (void)((Node*)node->mLeft->mParent == node);
            Destroy_unk(node->mLeft);
        }
        if (node->mRight != 0) {
            (void)((Node*)node->mRight->mParent == node);
            Destroy_unk(node->mRight);
        }
        delete node;
    }
    Node* End_unk() { return (Node*)&mRoot; }
    // inferred: the key compare of the map (std::less<float>; the result is kept as a bool)
    static bool Less_unk(const float& a, const float& b) { return a < b; }
    Node* FindInsertPos_unk(const float& key, Node*& pred, bool& left, bool& leftmost);
    Node* InsertNode_unk(Node* parent, bool left, bool leftmost, const typename Node::Value& value);
    // inferred: map::find
    Node* Find_unk(const float& key) {
        Node* result = End_unk();
        Node* node = mRoot;
        while (node != 0) {
            if (Less_unk(node->mValue.first, key)) {
                node = node->mRight;
            } else {
                result = node;
                node = node->mLeft;
            }
        }
        if (result == End_unk() || Less_unk(key, result->mValue.first)) {
            return End_unk();
        }
        return result;
    }
    // inferred: map::insert
    TeamDistResult_unk Insert_unk(const typename Node::Value& value) {
        Node* pred;
        bool left;
        bool leftmost;
        Node* parent = FindInsertPos_unk(value.first, pred, left, leftmost);
        if (pred == 0 || Less_unk(pred->mValue.first, value.first)) {
            return TeamDistResult_unk(InsertNode_unk(parent, left, leftmost, value), true);
        }
        return TeamDistResult_unk(pred, false);
    }
    static Node* Allocate_unk() {
        Node* p = (Node*)::operator new(sizeof(Node));
        if (p == 0) {
            __msl_error("Memory allocation failure");
        }
        return p;
    }
    // inferred: frees the node if the construction does not finish
    struct NodeHolder_unk {
        NodeHolder_unk(void* alloc, Node* node) : mAlloc(alloc), mNode(node) {}
        ~NodeHolder_unk() {
            if (mNode != 0) {
                delete mNode;
            }
        }
        Node* release() {
            Node* node = mNode;
            mNode = 0;
            return node;
        }
        void* mAlloc;
        Node* mNode;
    };

    unsigned int mSize;
    Node* mRoot;
    Node* mFirst;
};

template <class Node>
Node* TeamDistTreeData_unk<Node>::FindInsertPos_unk(const float& key, Node*& pred, bool& left, bool& leftmost) {
    pred = 0;
    Node* parent = End_unk();
    Node* node = mRoot;
    left = true;
    leftmost = true;
    while (node != 0) {
        parent = node;
        if (Less_unk(key, node->mValue.first)) {
            node = node->mLeft;
            left = true;
        } else {
            pred = node;
            node = node->mRight;
            left = false;
            leftmost = false;
        }
    }
    return parent;
}

template <class Node>
Node* TeamDistTreeData_unk<Node>::InsertNode_unk(Node* parent, bool left, bool leftmost,
                                                 const typename Node::Value& value) {
    if (mSize == 0xFFFFFFFF) {
        __msl_error("tree::insert length error");
    }
    NodeHolder_unk holder(&mRoot, Allocate_unk());
    ::new (&holder.mNode->mValue) typename Node::Value(value);
    Node* node = holder.release();
    node->mRight = 0;
    node->mLeft = 0;
    ::new (&node->mParent) unsigned int((unsigned int)parent);
    if (left) {
        parent->mLeft = node;
    } else {
        parent->mRight = node;
    }
    mSize++;
    std::__red_black_tree_void::balance_insert((std::__tree_node_base_void*)node, (std::__tree_node_base_void*)mRoot);
    if (leftmost) {
        mFirst = node;
    }
    return node;
}
// inferred: the layers of std::map over the tree (the destructor tests the object five times)
template <class Node>
struct TeamDistTreeL4_unk : public TeamDistTreeData_unk<Node> {
    ~TeamDistTreeL4_unk() {}
};
template <class Node>
struct TeamDistTreeL3_unk : public TeamDistTreeL4_unk<Node> {
    ~TeamDistTreeL3_unk() {}
};
template <class Node>
struct TeamDistTreeL2_unk : public TeamDistTreeL3_unk<Node> {
    ~TeamDistTreeL2_unk() {}
};
template <class Node>
struct TeamDistTree_unk : public TeamDistTreeL2_unk<Node> {
    ~TeamDistTree_unk() {}
};
typedef TeamDistTree_unk<TeamDistNode_unk> TeamDistMap_unk;

void TeamAI_unk::fn_805AE26C(int type, TeamData_unk* data) {
    mType = type;
    m218 = data;
    if (data->mAIType == 0) {
        SetAIType_unk(GetAIType_unk());
        SetAIType_unk(0);
    } else {
        SetAIType_unk(data->mAIType);
    }
    mTargetMode = 0;
    mTargetType = 0;
    m378 = 1;
    mTgtCtrlMode.Push_unk(0);
    m378 = 1;
    memset(m2D0, 0, sizeof(m2D0));
    fn_805AE3C4();
}

void TeamAI_unk::fn_805AE3C4() {
    memset(mMembers, 0, sizeof(mMembers));
    mNumMembers = 0;
    if (lbl_8087FA00->IsTop_unk(this)) {
        mNumMembers = fn_805AEC4C(mMembers, 0);
    }
    for (int i = 0; i < mNumMembers; i++) {
        mMembers[i]->mTeam = this;
    }
}

// inferred: HP of the members over their maximum, in percent
float TeamAI_unk::fn_805AE454() {
    int max = 0;
    float hp = 0.0f;
    for (int i = 0; i < mNumMembers; i++) {
        CharaTask* chara = Chara_unk(mMembers[i]);
        hp += chara->mStatus.m04;
        max += chara->mStatus.mParamB.m84;
    }
    if (max == 0) {
        return 0.0f;
    }
    return (100.0f * hp) / max;
}

// inferred: members that can still fight, in percent
float TeamAI_unk::fn_805AE5E8() {
    int count = 0;
    if (mType == 2) {
        for (int i = 0; i < mNumMembers; i++) {
            if (Chara_unk(mMembers[i])->IsActive_unk()) {
                count++;
            }
        }
    } else {
        for (int i = 0; i < mNumMembers; i++) {
            CharaTask* chara = Chara_unk(mMembers[i]);
            if (!chara->IsFlag_unk(4)) {
                if (chara->CanAct_unk() || chara->mStatus.m224 > 0) {
                    count++;
                }
            }
        }
    }
    if (mNumMembers == 0) {
        return 0.0f;
    }
    return (100.0f * count) / mNumMembers;
}

// inferred: HP of the leader, in percent
float TeamAI_unk::fn_805AE74C() {
    CharaTask* leader = (CharaTask*)GetLeader_unk();
    if (leader == 0) {
        return 0.0f;
    }
    return (100.0f * leader->mStatus.m04) / leader->mStatus.mParamB.m84;
}

// inferred: members that can act
int TeamAI_unk::fn_805AE7B4() {
    int count = 0;
    for (int i = 0; i < mNumMembers; i++) {
        if (Chara_unk(mMembers[i])->CanAct_unk()) {
            count++;
        }
    }
    return count;
}

// inferred: sets the HP of the members to `ratio` percent; the ones at 0 are knocked back
void TeamAI_unk::fn_805AE864(int ratio) {
    for (int i = 0; i < mNumMembers; i++) {
        CharaTask* chara = Chara_unk(mMembers[i]);
        chara->mStatus.m04 = (float)(ratio * chara->mStatus.mParamB.m84) / 100.0f;
        chara->mStatus.fn_801333E4();
        if (ratio <= 0 && lbl_8087F048 != 0) {
            atn::Vector3 dir(0.0f, 0.0f, 1.0f);
            Mtx m;
            PSMTXRotRad(m, 'y', chara->mRot.y);
            PSMTXMultVec(m, &dir, &dir);
            atn::Vector3 back(-dir.x, -dir.y, -dir.z);
            lbl_8087F048->fn_800FDE60(0, chara, back);
        }
    }
}

void TeamAI_unk::SetAIType_unk(unsigned int type) {
    if (type <= 2) {
        mAIType = type;
        m378 = 1;
    }
}

void TeamAI_unk::SetBattleMode_unk(int mode) {
    mBattleMode = mode;
    m378 = 1;
    for (int i = 0; i < mNumMembers; i++) {
        Chara_unk(mMembers[i])->mC58.mC0 = mode;
    }
}

void TeamAI_unk::SetAreaID_unk(int id) {
    mAreaID = id;
    m378 = 1;
    for (int i = 0; i < mNumMembers; i++) {
        Chara_unk(mMembers[i])->mC58.mB4 = id;
    }
}

void TeamAI_unk::SetM242_unk(int value) {
    m242 = value;
    for (int i = 0; i < mNumMembers; i++) {
        Chara_unk(mMembers[i])->mC58.mB8 = value;
    }
}

void TeamAI_unk::fn_805AEA70(TeamCmd_unk* cmd) {
    m10.PushFront_unk(*cmd);
}

void TeamAI_unk::fn_805AEAC0() {
    m10.mHead = 0;
    m10.mTail = 31;
    m23E = 0;
    for (int i = 0; i < mNumMembers; i++) {
        Chara_unk(mMembers[i])->fn_80171DB0();
    }
}

// inferred: the members have finished moving (mode 0/1: the ones that can act; 2: any one moving)
int TeamAI_unk::fn_805AEB34(int mode) {
    switch (mode) {
    case 0:
    case 1:
        for (int i = 0; i < mNumMembers; i++) {
            CharaTask* chara = Chara_unk(mMembers[i]);
            if ((chara->CanAct_unk() || mode == 0) && chara->mRoute.m2C == 0) {
                return 0;
            }
        }
        return 1;
    case 2:
        for (int i = 0; i < mNumMembers; i++) {
            if (Chara_unk(mMembers[i])->mRoute.m2C != 0) {
                return 1;
            }
        }
        return 0;
    }
    return 1;
}

// inferred: adds the characters of this team and of the teams below it to `list` (at most 32)
int TeamAI_unk::fn_805AEC4C(TeamChara_unk** list, int count) {
    if (count == -1) {
        return -1;
    }
    TeamChara_unk** out = &list[count];
    for (int i = 0; i < 32; i++) {
        TeamChara_unk* chara = m218->mCharas[i];
        if (chara != 0) {
            if (count >= 32) {
                return -1;
            }
            bool found = false;
            for (int j = 0; j < count; j++) {
                if (list[j] == chara) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                *out++ = chara;
                count++;
            }
        }
    }
    if (mChild != 0) {
        count = ((TeamAI_unk*)mChild)->fn_805AEC4C(list, count);
    }
    if (!lbl_8087FA00->IsTop_unk(this) && mNext != 0) {
        count = ((TeamAI_unk*)mNext)->fn_805AEC4C(list, count);
    }
    return count;
}

// NONMATCHING: 96.5% - the original loads the point before the counters are increased and keeps it in
// r3; same code otherwise (three instantiations)
template <class P>
int TeamAI_unk::SelectPoint_unk(int index, P** points, int mode) {
    int count = 0;
    while (count < 32 && points[count] != 0) {
        count++;
    }
    switch (mode) {
    case 0:
        return points[index % count]->mId;
    case 1: {
        int start = count * (rand() / 32767.0f);
        for (int i = 0; i < count; i++) {
            int k = (start + i) % count;
            if (m21C[k] <= m23C / count) {
                m21C[k]++;
                P* point = points[k];
                m23C++;
                return point->mId;
            }
        }
        break;
    }
    case 2: {
        float best = 3.4028235e38f;
        int found = -1;
        for (int i = 0; i < count; i++) {
            if (m21C[i] <= m23C / count) {
                const atn::Vector3& pos = points[i]->mPos;
                const atn::Vector3& from = Chara_unk(mMembers[index])->mPos;
                atn::Vector3 d(pos.x - from.x, pos.y - from.y, pos.z - from.z);
                float dist = d.LengthSq_unk();
                if (dist < best) {
                    found = i;
                    best = dist;
                }
            }
        }
        m21C[found]++;
        P* point = points[found];
        m23C++;
        return point->mId;
    }
    }
    return -1;
}

// NONMATCHING: 87.6% - smaller frame (the original keeps more vector temporaries on the stack) and the
// unused result of the inlined map::insert is not stored here; registers
// inferred: sends the team as a whole (mode 3: across the line from the members to the points, mode 4:
// along it): the members and the points are sorted by their position on the line and paired in order
// (three instantiations; names invented)
template <class P>
void TeamAI_unk::Move_unk(int kind, TeamPointList_unk<P>* list, unsigned int flags, int mode) {
    atn::Vector3 center(0.0f, 0.0f, 0.0f);
    int members = 0;
    for (unsigned int i = 0; i < mNumMembers; i++) {
        CharaTask* chara = Chara_unk(mMembers[i]);
        if ((flags & 4) == 4 && chara == GetLeader_unk()) {
            continue;
        }
        bool ok = false;
        if (!chara->IsFlag_unk(4)) {
            bool alive = true;
            if (chara->mType == 2 && chara->mStatus.IsState_unk(0x20)) {
                alive = false;
            }
            if (alive) {
                ok = true;
            }
        }
        if (ok) {
            members++;
            center += chara->mPos;
        }
    }
    if (members > 0) {
        center *= 1.0f / members;
    }
    atn::Vector3 target(0.0f, 0.0f, 0.0f);
    int count = 0;
    for (P** point = list->mPoints; *point != 0; point++) {
        count++;
        target += (*point)->mPos;
    }
    if (count > 0) {
        target *= 1.0f / count;
    }
    atn::Vector3 dir = target - center;
    atn::Vector3 axis;
    atn::Vector3 start;
    atn::Vector3 end;
    if (mode == 3) {
        atn::Vector3 up(0.0f, 1.0f, 0.0f);
        atn::Vector3 side;
        PSVECCrossProduct(&dir, &up, &side);
        atn::Vector3 mid = (target + center) * 0.5f;
        start = mid - side;
        end = mid + side;
        axis = side * 2.0f;
    } else if (mode == 4) {
        start = target;
        end = center;
        axis = target - center;
    }
    TeamDistMap_unk charas;
    TeamDistMap_unk points;
    for (unsigned int i = 0; i < mNumMembers; i++) {
        CharaTask* chara = Chara_unk(mMembers[i]);
        if ((flags & 4) == 4 && chara == GetLeader_unk()) {
            continue;
        }
        bool out = false;
        if (chara->IsFlag_unk(4) || (chara->mType == 2 && chara->mStatus.IsState_unk(0x20))) {
            out = true;
        }
        if (out) {
            continue;
        }
        float len = PSVECSquareMag(&axis);
        atn::Vector3 d = start - chara->mPos;
        float t = -PSVECDotProduct(&axis, &d) / len;
        if (charas.Find_unk(t) != charas.End_unk()) {
            t += 0.001f * ((rand() % 1001) / 1000.0f);
        }
        charas.Insert_unk(TeamDistPair_unk(t, (int)chara));
    }
    for (int i = 0; i < count; i++) {
        float len = PSVECSquareMag(&axis);
        atn::Vector3 d = start - list->mPoints[i]->mPos;
        float t = -PSVECDotProduct(&axis, &d) / len;
        if (points.Find_unk(t) != points.End_unk()) {
            t += 0.001f * ((rand() % 1001) / 1000.0f);
        }
        points.Insert_unk(TeamDistPair_unk(t, list->mPoints[i]->mId));
    }
    unsigned int num = points.mSize;
    if (charas.mSize < num) {
        num = charas.mSize;
    }
    TeamDistNode_unk* c = charas.mFirst;
    TeamDistNode_unk* p = points.mFirst;
    for (unsigned int i = 0; i < num; i++) {
        if (kind == 0) {
            ((CharaTask*)c->mValue.second)->fn_8016F824(p->mValue.second, 0);
        } else if (kind == 1) {
            ((CharaTask*)c->mValue.second)->fn_8016FDCC(p->mValue.second, 0);
        } else if (kind == 2) {
            ((CharaTask*)c->mValue.second)->fn_8017039C(p->mValue.second, 0);
        }
        c = c->Next_unk();
        p = p->Next_unk();
    }
}

// NONMATCHING: 98.7% - this/move/character in r27/r28/r29 in the original (r28/r29/r27 here)
void TeamAI_unk::fn_805AED74(TeamMove_unk* move) {
    if (!(m23E & 1)) {
        if (move->m0A == 3 || move->m0A == 4) {
            if (move->mKind == 0) {
                Move_unk(move->mKind, (TeamPointList_unk<TeamRoutePoint_unk>*)move->mList, move->mFlags, move->m0A);
            } else if (move->mKind == 1) {
                Move_unk(move->mKind, (TeamPointList_unk<TeamHidePoint_unk>*)move->mList, move->mFlags, move->m0A);
            } else if (move->mKind == 2) {
                Move_unk(move->mKind, (TeamPointList_unk<TeamPlace_unk>*)move->mList, move->mFlags, move->m0A);
            }
        } else {
            m23C = 0;
            memset(m21C, 0, sizeof(m21C));
            for (int i = 0; i < mNumMembers; i++) {
                CharaTask* chara = Chara_unk(mMembers[i]);
                if ((move->mFlags & 4) == 4 && chara == GetLeader_unk()) {
                    continue;
                }
                bool out = false;
                if (chara->IsFlag_unk(4) || (chara->mType == 2 && chara->mStatus.IsState_unk(0x20))) {
                    out = true;
                }
                if (out) {
                    continue;
                }
                switch (move->mKind) {
                case 0:
                    chara->fn_8016F824(
                        SelectPoint_unk(i, ((TeamPointList_unk<TeamRoutePoint_unk>*)move->mList)->mPoints, 2), 0);
                    break;
                case 1:
                    chara->fn_8016FDCC(
                        SelectPoint_unk(i, ((TeamPointList_unk<TeamHidePoint_unk>*)move->mList)->mPoints, 2), 0);
                    break;
                case 2:
                    chara->fn_8017039C(
                        SelectPoint_unk(i, ((TeamPointList_unk<TeamPlace_unk>*)move->mList)->mPoints, 2), 0);
                    break;
                default:
                    return;
                }
                if (move->mMode == 2 && m23C != 0) {
                    break;
                }
            }
        }
        m23E |= 1;
    } else if (fn_805AEB34(move->mMode)) {
        m23E = 0;
    }
}

// NONMATCHING: 99.85% - the original loads m0A into r7 (r5 here), as in fn_805AED74
void TeamAI_unk::fn_805B1918(TeamMove_unk* move) {
    if (!(m23E & 1)) {
        if (move->m0A == 3 || move->m0A == 4) {
            Move_unk(move->mKind, (TeamPointList_unk<TeamHidePoint_unk>*)move->mList, move->mFlags, move->m0A);
        } else {
            m23C = 0;
            memset(m21C, 0, sizeof(m21C));
            for (int i = 0; i < mNumMembers; i++) {
                CharaTask* chara = Chara_unk(mMembers[i]);
                if ((move->mFlags & 4) == 4 && chara == GetLeader_unk()) {
                    continue;
                }
                bool out = false;
                if (chara->IsFlag_unk(4) || (chara->mType == 2 && chara->mStatus.IsState_unk(0x20))) {
                    out = true;
                }
                if (out) {
                    continue;
                }
                switch (move->mKind) {
                case 1:
                    chara->fn_8016FDCC(
                        SelectPoint_unk(i, ((TeamPointList_unk<TeamHidePoint_unk>*)move->mList)->mPoints, 2), 0);
                    break;
                default:
                    return;
                }
                if (move->mMode == 2 && m23C != 0) {
                    break;
                }
            }
        }
        m23E |= 1;
    } else if (fn_805AEB34(move->mMode)) {
        m23E = 0;
    }
}

void TeamAI_unk::fn_805B1A9C(TeamVarOp_unk* op) {
    int a;
    switch (op->mDst) {
    case 0:
        a = lbl_8087F430->fn_80370A78(op->mVar);
        break;
    case 1:
        a = lbl_8087F430->fn_80370174(op->mVar);
        break;
    default:
        return;
    }
    int b;
    switch (op->mSrc) {
    case 0:
        b = op->mValue;
        break;
    case 1:
        b = lbl_8087F430->fn_80370A78(op->mValue);
        break;
    case 2:
        b = lbl_8087F430->fn_80370174(op->mValue);
        break;
    case 3:
        rand(); // inferred: a random source left unfinished
    default:
        return;
    }
    switch (op->mOp) {
    case 0:
        a = b;
        break;
    case 1:
        a = a + b;
        break;
    case 2:
        a = a - b;
        break;
    case 3:
        a = a * b;
        break;
    case 4:
        a = a / b;
        break;
    case 5:
        a = a == b;
        break;
    case 6:
        a = a != b;
        break;
    case 7:
        a = a < b;
        break;
    case 8:
        a = a <= b;
        break;
    case 9:
        a = a > b;
        break;
    case 10:
        a = a >= b;
        break;
    case 11:
        a = a & b;
        break;
    case 12:
        a = a | b;
        break;
    default:
        return;
    }
    switch (op->mDst) {
    case 0:
        lbl_8087F430->fn_80370AE4(op->mVar, a);
        break;
    case 1:
        lbl_8087F430->fn_80370320(op->mVar, a, 0);
        break;
    }
}

// inferred: the member whose data has m122 == 2, else the first one (name invented)
inline TeamChara_unk* TeamAI_unk::FindGimmickUser_unk() {
    for (int i = 0; i < mNumMembers; i++) {
        TeamChara_unk* chara = mMembers[i];
        if (Chara_unk(chara)->m5C->m122 == 2) {
            return chara;
        }
    }
    return mMembers[0];
}

void TeamAI_unk::fn_805B1C84(TeamGimmick_unk* order) {
    switch (order->mMode) {
    case 0:
    case 1:
        for (int i = 0; i < mNumMembers; i++) {
            CharaTask* chara = Chara_unk(mMembers[i]);
            if (chara->CanAct_unk() || order->mMode == 0) {
                lbl_8087EE68->fn_80017EB0(chara, order->mGimmick, order->mArg);
            }
        }
        break;
    case 2: {
        TeamChara_unk* chara = FindGimmickUser_unk();
        if (chara != 0) {
            lbl_8087EE68->fn_80017EB0(Chara_unk(chara), order->mGimmick, order->mArg);
        } else if (lbl_8087EEB8 != 0) {
            char buf[0x100];
            sprintf(buf, "[TeamAI] Error: ExecuteAimGimmick chara not found [gm%03d_%03d_%04d]\0",
                    order->mGimmick / 1000, order->mGimmick % 1000, order->mArg);
            lbl_8087EEB8->fn_800697D8(buf);
        }
        break;
    }
    }
}

void TeamAI_unk::fn_805B1E5C(TeamAttack_unk* order) {
    switch (order->mMode) {
    case 0:
    case 1:
        for (int i = 0; i < mNumMembers; i++) {
            CharaTask* chara = Chara_unk(mMembers[i]);
            if (chara->CanAct_unk() || order->mMode == 0) {
                lbl_8087F048->fn_80102A40(chara, order->mTarget, 100000, -1);
            }
        }
        break;
    case 2: {
        CharaTask* target = order->mTarget;
        CharaTask* nearest = Chara_unk(mMembers[0]);
        float best = 3.4028235e38f;
        if (mNumMembers > 0) {
        for (int i = 0; i < mNumMembers; i++) {
            CharaTask* chara = Chara_unk(mMembers[i]);
            atn::Vector3 d(target->mPos.x - chara->mPos.x, target->mPos.y - chara->mPos.y,
                           target->mPos.z - chara->mPos.z);
            float dist = d.LengthSq_unk();
            if (dist < best) {
                nearest = chara;
                best = dist;
            }
        }
        }
        lbl_8087F048->fn_80102A40(nearest, order->mTarget, 100000, -1);
        break;
    }
    }
}

// NONMATCHING: 99.6% - inlined fn_805AE864: index/character and the two vectors swapped (r27/r28,
// 0x8/0x14), as in fn_805AE864
void TeamAI_unk::fn_805B2028(TeamSetParam_unk* order) {
    int value;
    switch (order->mSrc) {
    case 0:
        value = order->mValue;
        break;
    case 1:
        value = lbl_8087F430->fn_80370A78(order->mValue);
        break;
    case 2:
        value = lbl_8087F430->fn_80370174(order->mValue);
        break;
    case 3:
        rand(); // inferred: a random source left unfinished
    default:
        return;
    }
    switch (order->mKind) {
    case 0:
        fn_805AE864(value);
        break;
    case 3:
        SetAIType_unk(value);
        break;
    case 4:
        SetBattleMode_unk(value);
        break;
    case 5:
        SetAreaID_unk(value);
        break;
    }
}

// NONMATCHING: 97.5% - inlined fn_805AE454 (8x unrolled sum): register allocation, as in fn_805AE454
void TeamAI_unk::fn_805B228C(TeamGetParam_unk* order) {
    int value;
    switch (order->mKind) {
    case 0:
        value = fn_805AE454();
        break;
    case 1:
        value = fn_805AE5E8();
        break;
    case 2:
        value = fn_805AE74C();
        break;
    case 3:
        value = mAIType;
        break;
    case 4:
        value = mBattleMode;
        break;
    case 5:
        value = mAreaID;
        break;
    default:
        return;
    }
    switch (order->mDst) {
    case 0:
        lbl_8087F430->fn_80370AE4(order->mVar, value);
        break;
    case 1:
        lbl_8087F430->fn_80370320(order->mVar, value, 0);
        break;
    }
}

// NONMATCHING: 97.9% - the original loads mHead again for the index of Last_unk (and in the member
// count of the inlined fn_805AE7B4 the counter is r5, r4 here)
void TeamAI_unk::fn_805B2560() {
    for (TeamCmd_unk* cmd = m10.Last_unk(); cmd != 0; cmd = m10.Last_unk()) {
        switch (cmd->m00) {
        case 28:
            fn_805AED74((TeamMove_unk*)&cmd->m04);
            break;
        case 29:
            fn_805B1918((TeamMove_unk*)&cmd->m04);
            break;
        case 32:
            fn_805B1A9C((TeamVarOp_unk*)&cmd->m04);
            break;
        case 33:
            fn_805B1C84((TeamGimmick_unk*)&cmd->m04);
            break;
        case 34:
            fn_805B1E5C((TeamAttack_unk*)&cmd->m04);
            break;
        case 36:
            fn_805B2028((TeamSetParam_unk*)&cmd->m04);
            break;
        case 35:
            fn_805B228C((TeamGetParam_unk*)&cmd->m04);
            break;
        }
        if (m23E & 1) {
            break;
        }
        m10.Pop_unk();
    }
    if (fn_805AE7B4() > 0) {
        fn_805B27A0();
    }
}

// NONMATCHING: 98.4% - registers, and the original walks the members of a team with a pointer
// inferred: target rates of the members toward the characters of the other teams (BattleChara_unk::m128):
// by the order of the target control (0: by mTargetType, 1: the leader is left alone)
void TeamAI_unk::fn_805B27A0() {
    if (m378 == 0) {
        return;
    }
    m378 = 0;
    BattleChara_unk* charas = lbl_8087F048->mCharas;
    if (charas == 0) {
        return;
    }
    float rates[0x48];
    int enemies[0x48];
    for (int i = 0; i < 0x48; i++) {
        rates[i] = 1.0f;
        enemies[i] = 0;
    }
    TeamAIManager* mgr = lbl_8087FA00;
    for (int t = 0; t < mgr->mTeams.mSize; t++) {
        TeamAI_unk* team = &mgr->mTeams.mData[t];
        if (mType == team->mType) {
            continue;
        }
        if (team->fn_805AE7B4() > 0) {
            for (int i = 0; i < team->mNumMembers; i++) {
                int index = lbl_8087F048->fn_80102890(Chara_unk(team->mMembers[i]));
                if (index >= 0) {
                    enemies[index] = 1;
                }
            }
        }
    }
    unsigned char ctrl = *mTgtCtrlMode.Last_unk();
    if (ctrl == 0) {
        switch (mTargetMode) {
        case 0:
            m245 = 2;
            break;
        case 1:
            m245 = 1;
            break;
        case 2:
            m245 = 4;
            break;
        }
        switch (mTargetType) {
        case 1: {
            // inferred: the enemy with the least hit points
            float best = 1e16f;
            int found = -1;
            for (int i = 0; i < 0x48; i++) {
                if (enemies[i] != 0 && charas[i].mChara->mStatus.m04 < best) {
                    best = charas[i].mChara->mStatus.m04;
                    found = i;
                }
            }
            if (found >= 0) {
                for (int i = 0; i < 0x48; i++) {
                    if (enemies[i] != 0) {
                        if (i == found) {
                            rates[i] = 2.0f;
                        } else {
                            rates[i] = 0.5f;
                        }
                    }
                }
            }
            break;
        }
        case 2: {
            // inferred: the enemy with the most hit points
            float best = 0.0f;
            int found = -1;
            for (int i = 0; i < 0x48; i++) {
                if (enemies[i] != 0 && charas[i].mChara->mStatus.m04 > best) {
                    best = charas[i].mChara->mStatus.m04;
                    found = i;
                }
            }
            if (found >= 0) {
                for (int i = 0; i < 0x48; i++) {
                    if (enemies[i] != 0) {
                        if (i == found) {
                            rates[i] = 2.0f;
                        } else {
                            rates[i] = 0.5f;
                        }
                    }
                }
            }
            break;
        }
        case 3:
            // inferred: the leaders of the teams
            for (int i = 0; i < 0x48; i++) {
                if (enemies[i] != 0) {
                    rates[i] = 0.5f;
                    for (int t = 0; t < mgr->mTeams.mSize; t++) {
                        if (charas[i].mChara == mgr->mTeams.mData[t].GetLeader_unk()) {
                            rates[i] = 2.0f;
                            break;
                        }
                    }
                }
            }
            break;
        case 4:
            for (int i = 0; i < 0x48; i++) {
                if (enemies[i] != 0) {
                    if (charas[i].mChara->fn_8014FCC4()) {
                        rates[i] = 2.0f;
                    } else {
                        rates[i] = 0.5f;
                    }
                }
            }
            break;
        case 5:
            for (int i = 0; i < 0x48; i++) {
                if (enemies[i] != 0) {
                    if (charas[i].mChara->fn_8014FCE8()) {
                        rates[i] = 2.0f;
                    } else {
                        rates[i] = 0.5f;
                    }
                }
            }
            break;
        case 6:
            for (int i = 0; i < 0x48; i++) {
                if (enemies[i] != 0) {
                    if (charas[i].mChara->fn_8014FD0C()) {
                        rates[i] = 2.0f;
                    } else {
                        rates[i] = 0.5f;
                    }
                }
            }
            break;
        }
    } else if (ctrl == 1) {
        int index = lbl_8087F048->fn_80102890(lbl_8087F8A0->mHead);
        if (index >= 0) {
            rates[index] = 0.0f;
        }
    }
    for (int i = 0; i < mNumMembers; i++) {
        BattleChara_unk* entry = lbl_8087F048->fn_80103F60(Chara_unk(mMembers[i]));
        if (entry != 0) {
            for (int j = 0; j < 0x48; j++) {
                if (enemies[j] != 0) {
                    entry->m128[j] = rates[j];
                }
            }
        }
    }
}
