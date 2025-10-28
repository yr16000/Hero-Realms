import csv
import re
from pathlib import Path

repo = Path(__file__).resolve().parent.parent
base_csv = repo / 'data' / 'base_set.csv'
mapping_csv = repo / 'data' / 'effect_mapping.csv'

# read mapping
mapping_raw_texts = set()
mapping_keys = {}
with mapping_csv.open(encoding='utf-8') as f:
    reader = csv.reader(f)
    headers = next(reader)
    for row in reader:
        if not row or len(row) < 2:
            continue
        raw_text = row[0].strip()
        key = row[1].strip() if len(row) > 1 else ''
        mapping_raw_texts.add(raw_text)
        mapping_keys[key] = raw_text

# extract candidate phrases from base set
brace_tokens = set()
line_phrases = set()
with base_csv.open(encoding='utf-8') as f:
    reader = csv.reader(f)
    headers = next(reader)
    for row in reader:
        if not row or len(row) < 4:
            continue
        text = row[3]
        # find tokens in braces
        for m in re.findall(r"\{[^}]+\}", text):
            brace_tokens.add(m.strip())
        # split on <hr> and newlines to get sentence-like phrases
        parts = re.split(r"<hr>|\\n|\\r\\n", text)
        for p in parts:
            p = p.strip()
            if not p:
                continue
            # normalize spaces
            pnorm = re.sub(r"\s+"," ", p)
            line_phrases.add(pnorm)

# also collect "You may" phrases
you_may_phrases = set([p for p in line_phrases if p.lower().startswith('you may')])

# compare
unmapped_brace = sorted([t for t in brace_tokens if t not in mapping_raw_texts])
unmapped_lines = []
for p in sorted(line_phrases):
    # consider mapped if any mapping_raw_text is substring of p or viceversa
    mapped = False
    for m in mapping_raw_texts:
        if m and (m in p or p in m):
            mapped = True
            break
    if not mapped:
        unmapped_lines.append(p)

# print report
print('Summary check of Base Set effects vs effect_mapping.csv')
print('-----------------------------------------------------')
print(f'Unique brace tokens found: {len(brace_tokens)}')
for t in sorted(brace_tokens):
    mapped = t in mapping_raw_texts
    print(f'  {t} -> in mapping: {mapped}')

print('\nYou may / conditional phrases found:')
for p in sorted(you_may_phrases):
    # try to see if mapping covers it
    covered = any(m in p or p in m for m in mapping_raw_texts)
    print(f'  {p} -> covered: {covered}')

print('\nLine-level phrases not matched by mapping heuristics (candidates to add):')
for p in unmapped_lines:
    print(' -', p)

# also list mapping entries that have no corresponding Effet class (simple check: existence of include/effets/<Class>.hpp)
print('\nChecking declared proposed_class existence for mapping entries:')
missing_classes = []
for key, raw in mapping_keys.items():
    # find the mapping row to get proposed class
    # naive parse: reopen mapping file
    pass

with mapping_csv.open(encoding='utf-8') as f:
    reader = csv.reader(f)
    next(reader)
    for row in reader:
        if not row or len(row) < 3:
            continue
        proposed = row[2].strip()
        if not proposed:
            continue
        # expected header file
        header = repo / 'include' / 'effets' / (proposed + '.hpp')
        if not header.exists():
            missing_classes.append(proposed)

if missing_classes:
    print('\nMapping proposes the following classes that are MISSING header files:')
    for c in sorted(set(missing_classes)):
        print('  ', c)
else:
    print('\nAll proposed classes have header files in include/effets (quick check).')

# exit code
print('\nDone.')
