import { fileURLToPath } from 'url';
import { dirname, resolve } from 'path';
import { createRequire } from 'module';
import { readFile } from 'fs/promises';

const require = createRequire(import.meta.url);
const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

function parseArgs() {
	const args = process.argv.slice(2);
	const params = {};
	for (let i = 0; i < args.length; i += 2) {
		if (args[i].startsWith('--')) {
			params[args[i].substring(2)] = args[i + 1];
		}
	}
	return params;
}

class Vosk {
	constructor() {
		this.binding = require(resolve(__dirname, '../build/Release/vosk_stt'));
		this.models = new Map();
	}

	createModel(modelPath) {
		if (!this.models.has(modelPath)) {
		const model = new this.binding.Model(modelPath);
		this.models.set(modelPath, model);
		}
		return this.models.get(modelPath);
	}

	createRecognizer(model, sampleRate = 16000) {
		if (!model) throw new Error('Model is required');
		return new this.binding.Recognizer(model, sampleRate);
	}

	async cleanup() {
		for (const model of this.models.values()) {
		try {
			model.free();
		} catch (err) {
			console.error('Error freeing model:', err);
		}
		}
		this.models.clear();
	}
}

async function main() {
	const { model, audio } = parseArgs();
	const MODEL_PATH = model;
	const AUDIO_FILE = audio;

	console.error('Starting recognition with:');
	console.error(`Model: ${MODEL_PATH}`);
	console.error(`Audio: ${AUDIO_FILE}`);

	const vosk = new Vosk();
	process.on('exit', () => vosk.cleanup());

	try {
		const model = vosk.createModel(MODEL_PATH);
		const recognizer = vosk.createRecognizer(model);
		const audioData = await readFile(AUDIO_FILE);

		const result = recognizer.acceptWaveform(audioData)
		? recognizer.result()
		: recognizer.partialResult();
		process.stdout.write(JSON.stringify(result));
	} catch (err) {
		console.error(JSON.stringify({ error: err.message }));
		process.exit(1);
	} finally {
		await vosk.cleanup();
	}
}

if (process.argv[1] === fileURLToPath(import.meta.url)) {
	main();
}