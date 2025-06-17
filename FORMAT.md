# Input / Output Format

## Input

- Two lists: **accepting** and **rejecting** sequences.
- Each list starts with an integer \( N \): number of sequences.
- For each sequence:
  - An integer \( M \): sequence length.
  - \( M \) binary digits (`0` or `1`), separated by tabs.

## Output

1. Accepting sequences (same format as input).
2. Rejecting sequences (same format as input).
3. DFA description:
   - Integer \( S \): number of states.
   - For each state (0 to \( S-1 \)):
     - Transition on `0` (state index)
     - Transition on `1` (state index)
     - Acceptance flag (`1` = accepting, `0` = rejecting)
