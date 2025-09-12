import 'dotenv/config';
import express from 'express';
import cors from 'cors';
import './mqtt/mqttclient.js';
import './database/db.js';
import temperatureRoutes from './routes.js';
import { errorHandler } from './middlewares/errorHandler.js';
import swaggerUi from 'swagger-ui-express';
import fs from "fs";

const swaggerDocument = JSON.parse(
  fs.readFileSync(new URL("./docs/swagger-output.json", import.meta.url))
);

const app = express();

app.use(cors());
app.use(cors({
  origin: 'http://localhost:port' //colocar seu ip e porta que esta rodando o frontend
}));


app.use(express.json());
app.use('/api', temperatureRoutes);
app.use('/api/docs', swaggerUi.serve, swaggerUi.setup(swaggerDocument));
app.use((error, req, res, next) => errorHandler(error, req, res, next));

app.get('/', (req, res) => {
  res.send('Servidor rodando!');
});


const PORT = process.env.PORT || 3000;

app.listen(PORT, () => {
  console.log(`Servidor rodando na porta ${PORT}`);
});

