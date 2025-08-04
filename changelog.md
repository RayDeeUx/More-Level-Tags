# EvenMoreLevelTags Changelog
## v1.5.0
- Use Geode layouts instead of insanity math for node positioning and node visibility logic.
- Fix a longstanding bug where settings from start position objects would be parsed as well.
## v1.4.1
- Experimental iOS support.
## v1.4.0
- Fix that one longstanding bug where the level tags would go offscreen when downloading a level for the first time.
- Added two additional tags: <cl>Shader Intolerance</c> and <cl>Camera Intolerance</c>.
  - Adjust your <cl>Shader Intolerance</c> and <cl>Camera Intolerance</c> thresholds separately in mod settings. Fully disable the intolerance tags by setting each threshold to a(ny) negative number.
  - Enable <cr>Advanced Tolerance Counter</c> if you'd like to fine-tune the requirements for showing these tags. <cr>(This red highlight should make it clear[er] that doing so is not recommended.)</c>
  - These will be the only "toggleable" tags.
## v1.3.3
- Port to 2.2074.
## v1.3.2
- Changed function hooks to minimize crashes.
- Port to 2.206. Big thanks to CatXus, Aktimoose, ninXout, and hiimjustin000 for helping out with testing!
- Added softtoggle.
- By default, tags will not show up when viewing levels published under the RobsVault account due to technical constraints. (This was not the case in earlier versions of this mod.)
- Added macOS support, which may be unstable. In case this mod consistently crashes on macOS, support for it will be dropped in future updates (hence the softtoggle).
## v1.3.1 (Note: was never published)
- Fix a few visual bugs and other things that weren't caught the first time around. 
## v1.3.0
- Bring back Android support.
- Refactor everything to avoid spawning phantom PlayLayers, and instead reading the level's decompressed string.
- Rebrand mod display name.
- Change mod ID, so that Globed's incompatibility list stays the same.
- Bump minimum Geode version and Geode Node ID requirements.
- Added more level tags (check the `about.md` for more info).
- License everything under the LGPLv2 license.
## v1.2.0
- Attempt to fix crashes from refreshing a level.
## v1.1.1
- Attempt to fix an unspecified crash.
## v1.1.0
- Fixed the invisible cursor glitch.
## v1.0.1
- Drop Android support due to crashes.
## v1.0.0
- Initial release.
