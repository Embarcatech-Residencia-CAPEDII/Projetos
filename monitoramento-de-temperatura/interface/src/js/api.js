
//--------------------TEMPERATURAS----------------
//buscar temperatura de todas as salas
export async function buscarTemperaturas() {
  const token = localStorage.getItem("token");
  const response = await fetch("http://localhost:3000/api/temperatures",
    {
      headers: {
        "Content-Type": "application/json",
        "Authorization": `Bearer ${token}`
      }
    });

  if (!response) {
    throw new Error(`Erro na requisição: ${response.status}`);
  }
  const dados = await response.json();
  return dados;
}


//buscar intervalo registrado de temperaturas de todas as salas
export async function TempIntervalo(start, end) {
  const token = localStorage.getItem("token");
  const response = await fetch(`http://localhost:3000/api/temperatures/interval?startDate=${start}&endDate=${end}`,
    {
      headers: {
        "Content-Type": "application/json",
        "Authorization": `Bearer ${token}`
      }
    });

  if (!response) {
    throw new Error("Erro ao buscar temperaturas");
  }
  const dados = await response.json();
  return dados;
}


//---------------SALAS-------------------------
//buscar temperatura de uma sala especifica
export async function buscarTemperaturaSala(salaId) {
  const token = localStorage.getItem("token");
  const response = await fetch(`http://localhost:3000/api/room/${salaId}/temperatures`,
    {
      headers: {
        "Content-Type": "application/json",
        "Authorization": `Bearer ${token}`
      }
    });

  if (!response) {
    throw new Error("Erro ao buscar temperaturas");
  }
  const dados = await response.json();
  return dados;
}

//buscar intervalo registrado de temperatura de uma sala especifica
export async function roomTempInterval(salaId, start, end) {
  const token = localStorage.getItem("token");
  const response = await fetch(`http://localhost:3000/api/room/${salaId}/temperatures/?startDate=${start}&endDate=${end}`,
    {
      headers: {
        "Content-Type": "application/json",
        "Authorization": `Bearer ${token}`
      }
    });

  if (!response) {
    throw new Error("Erro ao buscar temperaturas");
  }
  const dados = await response.json();
  return dados;
}

//Listar salas
export async function roomsSearch() {
  const token = localStorage.getItem("token");
  const response = await fetch(`http://localhost:3000/api/rooms`,
    {
      headers: {
        "Content-Type": "application/json",
        "Authorization": `Bearer ${token}`
      }
    });

  if (!response) {
    throw new Error("Erro ao buscar temperaturas");
  }
  const dados = await response.json();
  console.log(dados);
  return dados;
}


//----------------------USUARIOS--------------------
//Listar usuários do sistema
export async function usersSearch() {
  const token = localStorage.getItem("token");
  const response = await fetch(`http://localhost:3000/api/users`,
    {
      headers: {
        "Content-Type": "application/json",
        "Authorization": `Bearer ${token}`
      }
    });

  if (!response) {
    throw new Error("Erro ao buscar usuários");
  }
  const dados = await response.json();
  console.log(dados);
  return dados;
}

//Login de usuario
export async function userLogin(email, senha) {
  
    const body = {
        email: email,
        password: senha
    };

    try {
        // Faz a requisição de login
        const response = await fetch("http://localhost:3000/api/login", {
            method: "POST",
            headers: {
                "Content-Type": "application/json",
            },
            body: JSON.stringify(body),
        });

        const data = await response.json();
        console.log(data);

        if (response.ok) {
            localStorage.setItem("token", data.token);
            window.location.href = "index.html";

        } else {
            alert(data.message || "Usuário ou senha inválida.");
        }
    } catch (error) {
        console.error("Erro ao fazer login:", error);
        alert("Erro ao tentar fazer login. Tente novamente.");
        console.log(data.token);
    }
  }


  //Cadastrar usuarios no sistema
export async function userRegister(usuario, email, senha) {

  const token = localStorage.getItem("token");
  const body = {
    name: usuario,
    email: email,
    password: senha
  };
  try {
    const response = await fetch(`http://localhost:3000/api/user`,
      {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
          "Authorization": `Bearer ${token}`
        },
        body: JSON.stringify(body),
      });

    const data = await response.json();
    if (!response.ok) {
      throw new Error(error.message || "Erro ao cadastrar usuário");
    }
    return data;

  } catch (err) {
    console.error("Falha no cadastro:", err.message);
    throw err;
  }
}