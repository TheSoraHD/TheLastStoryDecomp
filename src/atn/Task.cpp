#include "atn/Task.h"

// Task tree of the engine: every task is linked to its parent (first child at 0x18, next sibling at
// 0x1C). The root is updated with fn_800D2494, drawn with fn_800D2AD8 and killed tasks are deleted with
// fn_800D2D7C (game/main). Function names are not known; the recursive helpers are static.

namespace atn {

} // namespace atn

// inferred: counters of the task system, defined in atn/TaskBase.cpp
extern "C" atn::TaskStats_unk lbl_807C75D8;

// inferred: set when a task is killed; fn_800D2D7C only walks the tree when it is set
static int lbl_8087EFF0;

// inferred: release assert of the original (the expression is evaluated, the result is ignored)
#define TASK_ASSERT_VALID(t) ((void)((t) != 0 && (t)->IsValid()))

// inferred: mapping tables used by the material and shader code (atn/MaterialParser_unk,
// atn/Shader_unk). They are at the start of the split; they may belong to another file.
extern "C" int fn_800D1C60(int mode) {
    if (mode == 0) {
        return 0;
    }
    if (mode == 1) {
        return 1;
    }
    if (mode == 2) {
        return 3;
    }
    if (mode == 3) {
        return 2;
    }
    if (mode == 6) {
        return 5;
    }
    if (mode == 5) {
        return 6;
    }
    if (mode == 4) {
        return 4;
    }
    return 7;
}

extern "C" int fn_800D1CD4(unsigned int mode) {
    switch (mode) {
    case 0:
        return 0;
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 3;
    case 4:
        return 4;
    case 5:
        return 5;
    case 6:
        return 6;
    case 7:
        return 7;
    }
    return 0;
}

namespace atn {

static void fn_800D2398(Task* task);
static void fn_800D26A4(Task* task);
static void fn_800D2868(Task* task);
static void fn_800D2C50(Task* task);
static void fn_800D32B8(Task* task);
static void fn_800D37B8(Task* task);
static void fn_800D3B74(Task* task);
static int fn_800D4280(Task* task);
static void fn_800D498C(Task* task);
static void fn_800D51EC(Task* task);

// inferred: killed by fn_800D2338
static inline bool IsKilled_unk(const Task* task) {
    return (task->mFlags & 0xDEAD0000) == 0xDEAD0000;
}

Task::Task(Task* parent) : TaskBase(0) {
    fn_800D1D94(parent);
}

// NONMATCHING: 95.00% - el recorrido hasta el ultimo hijo: el original compara *link en r0 y lo recarga
// (lwz/lwzu); probado: while, for, do-while y un accesor inline del enlace.
void Task::fn_800D1D94(Task* parent) {
    m18 = 0;
    m1C = 0;
    mFlags = 0;
    if (parent != 0) {
        TASK_ASSERT_VALID(parent);
        parent->AddChild_unk(this);
        Task* root = parent->m24;
        m24 = root;
        if (root != parent) {
            parent->m3C++;
        }
    } else {
        m24 = this;
        m20 = 0;
        mFlags |= 0x52;
    }
    m3C = 0;
    m28 = 0;
    TASK_U64(m30) = 0;
    m40 = 0;
    m24->m3C++;
    lbl_807C75D8.mCount++;
    m44 = 0;
}

// NONMATCHING: 95.44% - registros de los niveles inlineados del borrado recursivo (r29/r30/r31).
Task::~Task() {
    fn_800D37B8(this);
    if (m20 != 0) {
        ((Task*)m20)->RemoveChild_unk(this);
    }
    lbl_807C75D8.mCount--;
    m24->m3C--;
    if (m20 != 0 && m24 != m20) {
        m24->m3C--;
    }
    mFlags = 0;
}

// inferred: kills the task; it is deleted later by fn_800D2D7C
extern "C" void fn_800D2338(Task* task) {
    TASK_ASSERT_VALID(task);
    if (!IsKilled_unk(task)) {
        task->m40 = 1;
        lbl_8087EFF0 = 1;
        task->mFlags |= 0xDEAD0000;
    }
}

// inferred: destroys one task (notifies m28, then the deleter or delete)
static void fn_800D2398(Task* task) {
    task->mFlags |= 0x40;
    if (task->m28 != 0 && task->m28->IsValid() && TASK_U64(task->m30) == TASK_U64(task->m28->mId)) {
        task->m28->vf18(task);
        task->m28 = 0;
        TASK_U64(task->m30) = 0;
    }
    if (task->m44 == 0) {
        delete task;
    } else {
        task->m44(task);
    }
}

void Task::fn_800D246C(int arg) {
    if (arg) {
        mFlags |= 1;
    } else {
        mFlags &= ~1;
    }
}

// inferred: update of the whole tree
// NONMATCHING: 99.02% - r30/r31 intercambiados en los niveles inlineados de fn_800D26A4.
extern "C" void fn_800D2494(Task* root) {
    TASK_ASSERT_VALID(root);
    if (root->m20 == 0 || (root->mFlags & 0x10)) {
        if (!IsKilled_unk(root)) {
            if (!root->IsFlag_unk(1)) {
                fn_800D2868(root);
            } else {
                fn_800D26A4(root);
            }
        }
    }
}

// inferred: update of the children that only get vf0C (flag 1 of the parent)
static void fn_800D26A4(Task* task) {
    Task* child = task->m18;
    if (child != 0) {
        do {
            unsigned int flags = child->mFlags;
            if (!((flags & 0xDEAD0000) == 0xDEAD0000) && !(flags & 4)) {
                if (!(flags & 2)) {
                    if (child->vf0C()) {
                        child->mFlags |= 2;
                    }
                    flags = child->mFlags;
                }
                if (!(flags & 0xDEAD0004)) {
                    fn_800D26A4(child);
                }
            }
            if (child != 0) {
                child = child->m1C;
            }
        } while (child != 0);
    }
}

int Task::vf0C() {
    return 1;
}

// NONMATCHING: 99.17% - r30/r31 intercambiados en los niveles inlineados de fn_800D26A4.
static void fn_800D2868(Task* task) {
    Task* child = task->m18;
    if (child != 0) {
        do {
            unsigned int flags = child->mFlags;
            if (!((flags & 0xDEAD0000) == 0xDEAD0000) && !(flags & 4)) {
                if (!(flags & 2)) {
                    if (child->vf0C()) {
                        child->mFlags |= 2;
                    }
                    flags = child->mFlags;
                }
                if ((flags & 0xDEAD0007) == 2) {
                    child->vf10();
                    child->mFlags |= 0x20;
                }
                if (!(child->mFlags & 0xDEAD0004)) {
                    if (!child->IsFlag_unk(1)) {
                        fn_800D2868(child);
                    } else {
                        fn_800D26A4(child);
                    }
                }
            }
            if (child != 0) {
                child = child->m1C;
            }
        } while (child != 0);
    }
}

// inferred: draw of the whole tree
// NONMATCHING: 98.83% - el original sale con bne + b (aqui beq) y registros de los niveles inlineados.
extern "C" void fn_800D2AD8(Task* root) {
    TASK_ASSERT_VALID(root);
    if (root->m20 == 0 || (root->mFlags & 0x10)) {
        if (!IsKilled_unk(root)) {
            if (root->IsFlag_unk(4)) {
                return;
            }
            fn_800D2C50(root);
        }
    }
}

// NONMATCHING: 98.67% - r25/r26 intercambiados en el segundo y tercer nivel inlineado; probado: un helper
// inline por hijo (sale fuera de linea el helper), while, continue y mascara local.
static void fn_800D2C50(Task* task) {
    Task* child = task->m18;
    if (child != 0) {
        do {
            unsigned int mask = 0xDEAD0004;
            if (!(child->mFlags & mask)) {
                if ((child->mFlags & 0x22) == 0x22) {
                    child->vf14();
                }
                if (!(child->mFlags & mask)) {
                    fn_800D2C50(child);
                }
            }
            child = child->m1C;
        } while (child != 0);
    }
}

// inferred: deletes the killed tasks of the tree
// NONMATCHING: 90.93% - registros de los niveles inlineados y addic. -1 frente a subic. 1.
extern "C" void fn_800D2D7C(Task* root) {
    TASK_ASSERT_VALID(root);
    if (root->m20 == 0 || (root->mFlags & 0x10)) {
        if (lbl_8087EFF0 != 0) {
            fn_800D32B8(root);
        }
        lbl_8087EFF0 = 0;
    }
}

// NONMATCHING: 91.44% - registros de los niveles inlineados y addic. -1 frente a subic. 1.
static void fn_800D32B8(Task* task) {
    Task* child = task->m18;
    if (child != 0) {
        do {
            if (IsKilled_unk(child)) {
                if (--child->m40 > 0) {
                    continue;
                }
                fn_800D37B8(child);
                Task* next = child->m1C;
                fn_800D2398(child);
                child = next;
            } else if (child != 0) {
                fn_800D32B8(child);
                child = child->m1C;
            }
        } while (child != 0);
    }
}

// inferred: deletes all the descendants of the task
static void fn_800D37B8(Task* task) {
    Task* child = task->m18;
    if (child != 0) {
        fn_800D3B74(child);
    }
}

static void fn_800D3B74(Task* task) {
    TASK_ASSERT_VALID(task);
    if (task->m1C != 0) {
        fn_800D3B74(task->m1C);
    }
    if (task->m18 != 0) {
        fn_800D3B74(task->m18);
    }
    fn_800D2398(task);
}

void Task::fn_800D3F28(Task* child) {
    if (child != 0) {
        TASK_ASSERT_VALID(child);
        child->m28 = this;
        TASK_U64(child->m30) = TASK_U64(mId);
    } else if (m28 != 0) {
        m28 = 0;
        TASK_U64(m30) = 0;
    }
}

// inferred: nonzero when every descendant is ready (flag 2, set once its vf0C returns nonzero)
// NONMATCHING: 99.40% - registros de los niveles inlineados de fn_800D4280.
int Task::fn_800D3FA4() {
    Task* child = m18;
    if (child != 0) {
        return fn_800D4280(child);
    }
    return 1;
}

// NONMATCHING: 95.24% - el original guarda 4 GPR con stw (aqui stmw r27) y registros de los niveles.
static int fn_800D4280(Task* task) {
    int ready = 1;
    if (!task->IsFlag_unk(2)) {
        return 0;
    }
    Task* child = task->m18;
    if (child != 0) {
        TASK_ASSERT_VALID(child);
        ready = fn_800D4280(child);
    }
    if (ready) {
        child = task->m1C;
        if (child != 0) {
            TASK_ASSERT_VALID(child);
            ready = fn_800D4280(child);
        }
    }
    return ready;
}

// NONMATCHING: 42.70% - el original inlinea un nivel mas de fn_800D498C (assert del hijo en el llamador);
// con el assert al principio de la funcion llega a la profundidad pero cambia la entrada.
void Task::fn_800D4558() {
    Task* child = m18;
    if (child != 0) {
        fn_800D498C(child);
    }
}

// NONMATCHING: 42.73% - profundidad de inline (ver fn_800D4558).
static void fn_800D498C(Task* task) {
    if (task->m18 != 0) {
        TASK_ASSERT_VALID(task->m18);
        fn_800D498C(task->m18);
    }
    if (task->m1C != 0) {
        TASK_ASSERT_VALID(task->m1C);
        fn_800D498C(task->m1C);
    }
    task->mFlags |= 4;
}

// NONMATCHING: 42.70% - profundidad de inline (ver fn_800D4558).
void Task::fn_800D4DB8() {
    Task* child = m18;
    if (child != 0) {
        fn_800D51EC(child);
    }
}

// NONMATCHING: 42.73% - profundidad de inline (ver fn_800D4558).
static void fn_800D51EC(Task* task) {
    if (task->m18 != 0) {
        TASK_ASSERT_VALID(task->m18);
        fn_800D51EC(task->m18);
    }
    if (task->m1C != 0) {
        TASK_ASSERT_VALID(task->m1C);
        fn_800D51EC(task->m1C);
    }
    task->mFlags &= ~4;
}

} // namespace atn
