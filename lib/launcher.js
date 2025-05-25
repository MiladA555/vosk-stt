import { spawn } from 'child_process';
import { fileURLToPath } from 'url';
import { dirname, join } from 'path';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

export async function runVoskRecognition(options = {}) {
	return new Promise((resolve, reject) => {
		const {
			modelPath,
			audioFile,
			verbose = false
		} = options;
		if(!modelPath && !audioFile){
			reject(new Error('Err, Send modelPath and audioFile'));
		}
		const child = spawn(process.execPath, [
			fileURLToPath(new URL('./index.js', import.meta.url)),
			'--model', modelPath,
			'--audio', audioFile
		], {
			env: {
				...process.env,
				LD_LIBRARY_PATH: join(__dirname, '../third_party/lib'),
				VOSK_SILENT: '1' // Add this flag to suppress Vosk logs
			},
			stdio: ['inherit', 'pipe', 'pipe']
		});

		let resultData = '';
		let errorData = '';

		child.stdout.on('data', (data) => {
			resultData += data.toString();
		});

		child.stderr.on('data', (data) => {
			errorData += data.toString();
		});

		child.on('close', (code) => {
			if (code === 0) {
				try {
					resolve(JSON.parse(resultData.trim()));
				} catch (e) {
					reject(new Error('Failed to parse result'));
				}
			} else {
				reject(new Error(`Recognition failed: ${errorData}`));
			}
		});

		child.on('error', (err) => {
			reject(err);
		});
	});
}
