---
name: continual-learning
description: Incrementally extract recurring user corrections and durable workspace facts from this session's messages, then update AGENTS.md with plain bullet points only. Use when the user asks to mine previous chats, maintain AGENTS.md memory, or build a self-learning preference loop.
---

# Continual Learning

Keep `AGENTS.md` current by mining this session's conversation history.

## Inputs

- Current session messages (already in context)
- Existing memory file: `AGENTS.md`

## Workflow

1. Read existing `AGENTS.md`.
2. Review messages from the current session conversation.
3. Extract only high-signal, reusable information:
   - Recurring user corrections/preferences
   - Durable workspace facts (patterns, conventions, tech choices)
4. Merge with existing bullets in `AGENTS.md`:
   - Update matching bullets in place
   - Add only net-new bullets
   - Deduplicate semantically similar bullets
5. Write back to `AGENTS.md` with only these two sections added or updated:
   - `## Learned User Preferences`
   - `## Learned Workspace Facts`

## AGENTS.md Output Contract

- Manage only these sections:
  - `## Learned User Preferences`
  - `## Learned Workspace Facts`
- Use plain bullet points only.
- Do not write evidence/confidence tags.
- Do not write process instructions, rationale, or metadata blocks.
- Maximum 12 bullets per section.

## Inclusion Bar

Keep an item only if all are true:

- Actionable in future sessions
- Stable across sessions
- Repeated in the conversation, or explicitly stated as a broad rule
- Non-sensitive

## Exclusions

Never store:

- Secrets, tokens, credentials, private personal data
- One-off task instructions
- Transient details (branch names, commit hashes, temporary errors)
