# Release Preparation v0.5.0 — A6 Archive Construction and Extraction

Status: PASS

## Archive

- Archive: `release-assets/STALKER2UltrawideFix-UE5.5.4-v0.5.0-A6.zip`
- Archive SHA-256: `A41E973187AECF5AE4F02885C6E5ED3E32318E72ED08A2E4B2DA1616DB6E76E2`
- The archive was created as a new file; the historical v0.5.0 ZIP was not
  overwritten.

## Exact archive contents

The archive contains exactly these five root-level files:

```text
LICENSE.md
README.md
STALKER2UltrawideFix.asi
STALKER2UltrawideFix.ini
THIRD_PARTY_NOTICES.md
```

No nested ZIP, source, research, plan, log, debug or additional ASI file was
included.

## Extraction verification

The new archive was extracted into a clean temporary directory. The extracted
file set matched the A5 whitelist exactly, and every extracted file matched its
source asset SHA-256:

```text
STALKER2UltrawideFix.asi   B8A05E6DA92DDE89FBB213367B02F40BE3A5AACD46DC0C09E5D29DE9B13A881E
STALKER2UltrawideFix.ini   09DC837B4E32AC667B33D33FE7A3E6EA37201C215C4EC860A504ED523419563B
README.md                  CFEFFE77367ECDFCA3982022525090DE1BF9D28C8F8BA72FA2E36D78E7A6746D
LICENSE.md                 C13956C28013A89F6CFCA14EB626DCC47F5676E7A22D17885DB17F002F7CCC4D
THIRD_PARTY_NOTICES.md     F6DD1CF0587D073A80B58500611E79D6566234048EBACB72FD25FB23D8D49675
```

The extracted ASI SHA matches the A2/A5 approved production SHA exactly.
Temporary staging and extraction directories were removed after verification.

## A6 gate

The new archive was built exclusively from the A5 whitelist, extracted into a
clean directory, and verified by exact file-set and per-file hash comparison.
No archive content was edited during verification.

Archive construction and extraction: PASS.

Next permitted batch: A7 — Audit-only final consistency.
