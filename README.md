---

# 🎤 Vosk STT Node.js Bindings  
**Fast and accurate speech-to-text for Node.js using Vosk**

[![License: AGPL-3.0](https://img.shields.io/badge/License-AGPL--3.0-blue.svg)](https://opensource.org/licenses/AGPL-3.0)  

---

## 📦 Installation  
```bash
npm install vosk-stt
```

### 🔈 Audio Requirements  
Vosk requires **mono (1-channel) 16kHz 16-bit PCM WAV** files. Convert your audio using FFmpeg:

```bash
ffmpeg -i input.wav -ac 1 -ar 16000 -sample_fmt s16 output.wav
```

| Parameter       | Value | Description                  |
|-----------------|-------|------------------------------|
| `-ac 1`         | 1     | Mono audio                   |
| `-ar 16000`     | 16000 | 16kHz sample rate            |
| `-sample_fmt s16` | s16   | 16-bit PCM format            |

---

## 🚀 Usage  
### Basic Recognition  
```javascript
import { runVoskRecognition } from 'vosk-stt';

const result = await runVoskRecognition({
  modelPath: './model/english',
  audioFile: './converted_audio.wav' // Must be mono 16kHz
});
```

### Verify Audio Format  
Check your WAV file properties with:
```bash
ffprobe -v error -show_streams -select_streams a input.wav
```
**Look for:**  
```
channels=1  
sample_rate=16000  
sample_fmt=s16
```

---

## 🧪 Testing  
Test files **must** be in the correct format. Example test setup:

```bash
# Convert stereo to mono for testing
ffmpeg -i test/fixtures/original.wav -ac 1 -ar 16000 test/fixtures/test.wav
```

Then run:
```bash
npm test
```

---

## 🛠️ Troubleshooting  
**Error:** "Invalid audio format"  
1. Confirm mono channel count:  
   ```bash
   ffmpeg -i input.wav -ac 1 output.wav
   ```
2. Resample to 16kHz:  
   ```bash
   ffmpeg -i input.wav -ar 16000 output.wav
   ```

---

### 🔄 Batch Conversion Script  
Create `convert_audio.sh` for processing multiple files:
```bash
#!/bin/bash
mkdir -p converted
for file in ./input_audio/*.wav; do
  ffmpeg -i "$file" -ac 1 -ar 16000 "./converted/$(basename "$file")"
done
```

Run with:  
```bash
chmod +x convert_audio.sh
./convert_audio.sh
```

---

This update ensures users understand the strict audio requirements and provides ready-to-use conversion commands. The FFmpeg examples cover:
- Basic single-file conversion
- Audio verification
- Batch processing
- Troubleshooting steps

Would you like me to add any specific advanced audio processing scenarios?
