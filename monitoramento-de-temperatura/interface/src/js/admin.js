// import { checkAcess } from './main.js';
import { roomsSearch, usersSearch, userRegister } from './api.js';

// --- verifica permissões ---
// document.addEventListener("DOMContentLoaded", () => {
//   checkAcess("admin");
// });

const links = document.querySelectorAll('.menu-lateral a[data-section]');


links.forEach(link => {

  //eventListener para criar o conteudo da pagina dinamicamente de acordo com a opção do menu lateral do dashboard
  link.addEventListener('click', async (e) => {
    e.preventDefault();

    // Remove os cards de temperatura apos clicar nas opções menu lateral
    const roomsGrid = document.getElementById('roomsGrid');
    if (roomsGrid) {
      roomsGrid.remove();
    }


    const section = link.dataset.section;
    console.log(section);

    if (section === 'room') {
      try {
        const dados = await roomsSearch();

        if (!dados.length) {
          console.error("Nenhum dado encontrado.");
          return;
        }

        const grid = document.getElementById("dashboard");
        grid.innerHTML = "";

        const table = document.createElement("table");
        table.classList.add("table", "table-striped", "table-bordered");


        table.innerHTML = `
                            <caption>Salas</caption>
                            <thead>
                                <tr>
                                <th scope="col">Sala</th>
                                <th scope="col">Microcontrolador</th>
                                <th scope="col">ID</th>
                                <th scope="col">Ações</th>
                            </tr>
                            </thead>
                            <tbody></tbody>
                            `;

        const tbody = table.querySelector("tbody");


        dados.forEach(sala => {
          const row = document.createElement("tr");
          row.innerHTML = `
                            <td>${sala.name.toUpperCase()}</td>
                            <td>${sala.microcontroller}</td>
                            <td>${sala._id}</td>
                            <td>
                            <button class="btn-warning onclick="editarSala('${sala._id}')">Editar</button>
                            <button  data-id="${sala._id}" class="btn excluir">Excluir</button>
                            </td>
                            `;
          tbody.appendChild(row);
        });

        grid.appendChild(table);

        const button = document.getElementById('btn-click');
        button.classList.remove('hide');
        button.classList.add('show');
      } catch (error) {
        console.error("Erro ao carregar salas:", error);

        if (error.message.includes("401") || error.message.includes("403")) {
          alert("Acesso não autorizado. Faça login novamente.");
          localStorage.removeItem("token");
          window.location.href = "login.html";
        }
      }


    } else if (section === 'user') {
      try {
        const dados = await usersSearch();

        if (!dados.length) {
          console.error("Nenhum dado encontrado.");
          return;
        }
        const grid = document.getElementById("dashboard");
        grid.innerHTML = "";

        const table = document.createElement("table");
        table.classList.add("table", "table-striped", "table-bordered");
        table.innerHTML = `
                            <caption>Usuários</caption>
                            <thead>
                                <tr>
                                    <th scope="col">Nome</th>
                                    <th scope="col">Email</th>
                                    <th scope="col">ID</th>
                                    <th scope="col">Ações</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        `;

        const tbody = table.querySelector("tbody");


        dados.forEach(sala => {
          const row = document.createElement("tr");
          row.setAttribute("data-id", sala._id);
          row.innerHTML = `
          <td>${sala.name.toUpperCase()}</td>
          <td>${sala.email}</td>
          <td>${sala._id}</td>
          <td>
          <button type="button" class="btn-warning onclick="editarSala('${sala._id}')">Editar</button>
          <button class="btn excluir" data-id="${sala._id}">Excluir</button>
        </td>
        `;
          tbody.appendChild(row);
        });
        grid.appendChild(table);

        // Alterar a classe do botão para mostrar o conteúdo
        const button = document.getElementById('btn-click');
        button.classList.remove('hide');
        button.classList.add('show');

      } catch (error) {
        console.error("Erro ao carregar salas:", error);

        if (error.message.includes("401") || error.message.includes("403")) {
          alert("Acesso não autorizado. Faça login novamente.");
          localStorage.removeItem("token");
          window.location.href = "login.html";
        }
      }
    } else if (section === 'reports') {
      try {
        // const dados = await TempIntervalo(start, end);
        // console.log(dados);

        // if (!dados.length) {
        //   console.error(`Nenhum dado encontrado para o período`);
        //   window.alert("Sem dados para o período");
        //   return;
        // }

        const grid = document.getElementById("dashboard");
        grid.innerHTML = "";

        const relatorio = document.createElement("div");
        relatorio.classList.add("relatorio");
        relatorio.innerHTML = `
                <h3>Emitir Relatório</h3>
                <div>
                    <label>Escolha o local</label>
                    <select name="room" id="room-select">
                      <option value="">--Por favor escolha uma sala--</option>
                      <option value="0">Todas</option>
                      <option value="1">Sala-01</option>
                      <option value="2">Sala-02</option>
                      <option value="3">Sala-03</option>
                    </select>
                    <label for="periodo">Escolha o período:</label>
                    <input id="start" class="date" type="date">
                    <label>à </label>
                    <input id="end" class="date" type="date">
                    <button  type="button" class="btn" id="emitirRelatorio">Baixar</button>
                </div>
                        `;

        grid.appendChild(relatorio);

        // Alterar a classe do botão para mostrar o conteúdo
        const button = document.getElementById('btn-click');
        button.classList.remove('show');
        button.classList.add('hide');


      } catch (error) {
        console.error("Erro ao carregar sala:", error);

        if (error.message.includes("401") || error.message.includes("403")) {
          alert("Acesso não autorizado. Faça login novamente.");
          localStorage.removeItem("token");
          window.location.href = "login.html";
        }
      }
    }

    else if (section === 'dashboard') {
      location.reload();
    }
  });
});


// Evento para cadastrar
document.getElementById('btn-click').addEventListener('click', () => {

  // Mostrar o modal
  const modal = (document.getElementById('userInsert'));
  modal.showModal();

  //Fechar modal
  const btn_close = modal.querySelector("#userCloseBtn");
  btn_close.onclick = function () {
    modal.close();
  }

  const cadastrar = document.getElementById("userInsertBtn");
  cadastrar.onclick = async () => {
    const usuario = document.getElementById("name").value;
    const email = document.getElementById("email").value;
    const senha = document.getElementById("senha").value;
    const senha2 = document.getElementById("senha2").value;

    if (senha !== senha2) {
        const alerta = document.getElementById("response");
        alerta.innerText = "As senhas devem ser iguais!";
        
        // Limpa o texto de alerta depois de 3 segundos
        setTimeout(() => {alerta.innerText = "";}, 3000);
      return;
    }

    try {
      //requisicao da API para cadastrar usuario
      const data = await userRegister(usuario, email, senha) ;
      console.log(data);

      if (data && data.id) {
        const alerta = document.getElementById("response");
        alerta.innerText = `Usuário ${data.name} cadastrado com sucesso!`;

        // Limpa o texto de alerta depois de 3 segundos
        setTimeout(() => {alerta.innerText = "";}, 3000);

        //Limpa o formulário
        document.getElementById("userForm").reset();
      } else {

        // alerta de erro;
        const alerta = document.getElementById("response");
        alerta.innerText = data.erro || data.message || "Erro desconhecido no cadastro";
        // Limpa o texto depois de 3 segundos
        setTimeout(() => {alerta.innerText = "";}, 3000);
      }
    } catch (error) {
      alert(`Erro: ${error.message}`);
    }
  };
});


// Função para excluir sala ou usuário
document.addEventListener('click', function (e) {
  if (e.target && e.target.classList.contains('excluir')) {
    const id = e.target.getAttribute('data-id');
    excluirSala(id);
  }
});

async function excluirSala(id) {
  // Confirmar exclusão
  const confirmacao = confirm("Tem certeza que deseja excluir?");
  if (!confirmacao) return; // Se o usuário não confirmar, a função é encerrada

  try {
    const token = localStorage.getItem("token");

    // Requisição para excluir o item 
    const response = await fetch(`http://localhost:3000/api/user/${id}`, {
      method: "DELETE",
      headers: {
        "Content-Type": "application/json",
        "Authorization": `Bearer ${token}`,
      },
    });


    if (response.status === 204) {
      alert(`Item com ID ${id} excluído com sucesso!`);

      // Atualiza a tabela sem recarregar a página
      removerLinhaTabela(id);
      return;
    }

    const data = await response.json();

    if (response.ok) {
      alert(`Item com ID ${id} excluído com sucesso!`);

      // Atualiza a tabela sem recarregar a página
      removerLinhaTabela(id);
    } else {
      alert(data.erro || data.message || "Erro desconhecido ao excluir o item");
    }
  } catch (error) {
    alert(`Erro: ${error.message}`);
  }
}

// Função para remover a linha da tabela
function removerLinhaTabela(id) {
  const linha = document.querySelector(`tr[data-id='${id}']`);
  if (linha) {
    linha.remove();
  } else {
    console.error("Linha não encontrada!");
  }
}


//Evento para logout
document.getElementById('logoutBtn').addEventListener('click', () => {
  alert('Logout realizado!');
});