let hastalar = http://JSON.parse(localStorage.getItem('hastalar') || '[]');
const form = http://document.getElementById('form');
const liste = http://document.getElementById('liste');
const sayac = http://document.getElementById('sayac');
function goster(dizi = hastalar) {
  http://liste.innerHTML = '';
  http://dizi.forEach((h,i)=>{
    http://liste.innerHTML += `<tr><td>${h.ad}</td><td>${h.tc}</td><td>${h.bolum}</td><td>${h.tarih}</td><td>${h.saat}</td><td><span class="delete" onclick="sil(${i})">Sil</span></td></tr>`;
  });
  http://sayac.innerText = http://dizi.length;
  http://localStorage.setItem('hastalar', http://JSON.stringify(hastalar));
}
http://form.addEventListener('submit', (e)=>{
  http://e.preventDefault();
  const tc = http://document.getElementById('tc').value;
  if(tc.length !== 11 || !/^\d+$/.test(tc)){ alert('TC 11 rakam olmali'); return; }
  http://hastalar.push({
    ad: http://document.getElementById('ad').value,
    tc: tc,
    bolum: http://document.getElementById('bolum').value,
    tarih: http://document.getElementById('tarih').value,
    saat: http://document.getElementById('saat').value
  });
  goster();
  http://form.reset();
});
function sil(i){ if(confirm('Silinsin mi?')){ http://hastalar.splice(i,1); goster(); } }
function ara(){
  const q = http://document.getElementById('search').value.toLowerCase();
  goster(hastalar.filter(h=> http://h.ad.toLowerCase().includes(q) || http://h.tc.includes(q) || http://h.bolum.toLowerCase().includes(q)));
}
goster();
