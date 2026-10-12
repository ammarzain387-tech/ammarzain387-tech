Build my complete animated GitHub profile — banner, stats cards, contribution snake, and social
badges. I've attached my photo and logo references. Work through the four phases below in
order, and check in with me after each one. Don't generate five variations at once; show me one
and let me react.
My details
• 
• 
• 
Name: 
main )
Role: 
[Zain ul Abideen Ammar] · GitHub username: ammarzain387-tech
[Data Analyst]
Location: 
[ammarzain387-tech] (profile repo is ammarzain387-tech
[Rawalpindi, Pakistan] · Education:  
• 
• 
• 
• 
• 
Status: 
[Building + Learning + Programming]
ToolChain: 
Languages: 
LinkedIn 
[...] · Frontend: 
soon"]
[Computer Sciene]
[ VS Code, Git, Android Studio, Figma]
[...] · Backend: 
ammarzain387-tech/ammarzain387-tech , branch
Three logos to morph between: 
[Flutter, a </> glyph, Vercel] — I'm attaching
reference images; trace them, don't hand-draw them
1
• 
Palette: portrait 
[#A78BFA dark / #7C3AED light] · UI chrome 
[#10B981] · background 
[#0A101F]
[#22D3EE / #0891B2] · accent 
Palette rule: the portrait must be a different hue from the UI chrome, or the face blends into its
own frame.
PHASE 1 — Banner (dark.svg / light.svg)
One terminal window, 1180×610, titled 
labelled 
VISUAL.MAP . Right is a 
profile.sh --live . Left ~38% is a portrait frame
• 
• 
• 
SYSTEM.INFO readout with dotted leaders, a pulsing red LIVE
badge, and a coloured pill with my handle.
Portrait — build this in Python
Crop head + shoulders, not a tight face crop (over-zoomed reads aggressive)
300×340 grid, then 1-bit Floyd–Steinberg dither, serpentine order
Contrast 1.3× only, with 
• 
Draw dots as 
autocontrast(cutoff=1) + 
<path> runs with 
UnsharpMask(radius=3, percent=140)
shape-rendering="crispEdges" — never font glyphs, they
mush below ~2px
• 
• 
• 
• 
Dark mode: segment the background out (threshold on colour distance, binary closing, fill
holes, keep largest component) so dots draw the lit subject on the panel. Hard-clear error
diffusion bleed at the mask edge. Without this, dark mode looks like a photo negative
Light mode: keep the background; dots draw the dark parts of the photo
Single hue — all tone from dot density
No grid lines, scanlines, glitch bars, or CRT flicker
Animation
Intro (~3.2s, once): ~60 interleaved random groups fade in over ~2s. Each group must be
scattered across the whole portrait so dots appear everywhere at once and thicken together. Do 
not use a wipe. Do not group by spatial region — that reveals patch-by-patch instead of
shimmering in. Verify with an evenness metric (~0.05 good, ~0.7 patchy). Needs a duplicate
portrait layer (~180KB); merging to one layer breaks it.
Loop (~14.2s): portrait 3.0s, each logo 2.0s, 1.3s transitions. Use explicit uneven 
evenly-spaced keyframes force every phase to hold the same length.
Two independent layers:
1. 
2. 
keyTimes —
Portrait — full density (~17k dots), grouped into ~94 drift bands. On the loop each band
translates ~42% toward the first logo's centroid while fading, then returns
Travellers — ~900 dots that morph between logos, matched by optimal transport so each
takes the shortest path. Opacity keyframes 
0;0;0;1;1;...;0 so they're hidden during the
portrait phase — otherwise their thicker dots crowd the fine dither
The trap that will bite you: drift is a linear function of position, so quantizing it into groups
mathematically recreates a square grid — and the dissolve looks blocky. Add per-dot noise (sigma
~4) before grouping. Verify with a straight-boundary metric: ~0.01 organic, ~0.17 means you built
a grid.
2
Info panel
Rows at font-size 14, header 13, LIVE 12, pill 14, spacing 23px
Lock every row with textLength + lengthAdjust="spacingAndGlyphs" so values stay right
aligned in any browser font
Dotted leaders computed from label/value length — never hand-edit the SVG
Rows: Subject, Role, Origin, Education, Status, ToolChain · Core.Lang, Core.Frontend,
Core.Backend, Core.Database, Core.Infra · Grid.Mail, Grid.Portfolio, Grid.LinkedIn, Grid.GitHub,
Grid.Facebook
PHASE 2 — Stats cards (self-hosted)
Walk me through self-hosting github-readme-stats — don't just hand me public-instance URLs.
The public instance is shared by thousands and constantly returns "API rate limit exceeded". Give
me these steps explicitly:
Create a GitHub classic token: Settings, Developer settings, Tokens (classic), Generate new
(classic), repo scope, No expiration. Warn me to copy it immediately and never paste it
anywhere public
Fork anuraghazra/github-readme-stats
Vercel, sign up with GitHub, Hobby (free), Add New Project, import the fork
Add environment variable PAT_1 = my token, then Deploy
Ask me for my instance URL, then generate the themed block
Then produce: a streak card (streak-stats.demolab.com) at width="100%", plus stats and top
langs side by side at width="49%". Theme everything to my palette. Include hide_rank=true —
the rank is stars-weighted and misleading for newer accounts. Explain why rather than just doing
it.
PHASE 3 — Contribution snake
Write me .github/workflows/snake.yml using Platane/snk/svg-only@v3, on a 12-hour cron plus 
workflow_dispatch plus push to main, pushing to an output branch via crazy-max/ghaction
github-pages@v3.1.0. Include permissions: contents: write.
Tell me to set repo Settings, Actions, General, Workflow permissions, Read and write,
and be explicit that this is the repo's settings, not my account settings.
Two output SVGs — light and dark — themed to my palette. The first colour in color_dots is
the empty cell. For the dark snake it must be a visible slate like #2d3343: against GitHub's 
#0d1117 background a near-black empty cell disappears and the grid looks broken. Display via a
theme-aware <picture>, and tell me to only add it after the Action runs green — the output
branch doesn't exist before then.
PHASE 4 — Social badges
shields.io badges, for-the-badge style, my background colour, &nbsp;&nbsp; between each, all
clickable.
• 
• 
• 
1. 
2. 
3. 
4. 
5. 
3
#0A66C2 . On any
Warn me about the LinkedIn bug: its logo only renders on brand blue 
custom colour the glyph silently vanishes, leaving just text. Either use brand blue or embed the
glyph as a base64 data-URI to keep it themed. Other logos (Instagram, Gmail, Facebook) recolour
fine.
Skip a GitHub badge — it's circular on my own profile.
FINALLY — assemble
Give me the complete README in one block: banner 
badges, with every 
<picture> , then stats, then snake, then
USERNAME filled in. Then a short checklist of what I do by hand (upload SVGs,
create the token, deploy Vercel, enable Actions permissions).
How to work with me
• 
• 
• 
• 
• 
• 
Verify by measurement, not by eye. cairosvg renders only the first SMIL frame and
mishandles additive transforms and 
textLength . Use correlation vs the approved render,
band distributions, ink coverage — then tell me to check in a browser
When I say something "didn't change," check the file first: 
raw.githubusercontent.com/.../file.svg?v=999 , view-source, search the hex. It's almost
always CDN cache, not a bug. Also check I'm in the right theme — dark assets only render in
dark mode
Flag file size honestly. The banner lands ~900KB–1MB. Warn me before expensive changes
Tell me when I'm wrong. If an idea won't work or costs more than it's worth, say so instead
of building it
If I reject something twice, stop and ask rather than trying a third variation
Keep the generator script and 
▲ END OF PROMPT ▲
.npy data — they're the source of truth, not the SVG
