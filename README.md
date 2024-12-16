# Zindan Keşfi ve Hayatta Kalma Oyunu

Bu oyun, basit bir zindan keşfi ve hayatta kalma oyunu olup, oyuncuların çeşitli odalarda ilerleyerek, canavarlara karşı savaşıp, eşyalar topladığı bir oyun deneyimi sunar.

## Oynanış

Oyuncu, belirli bir odada başlar ve odalar arasında hareket eder. Oyuncunun sağlık, güç ve envanter gibi özellikleri vardır. Her odada farklı özellikler ve canavarlar bulunabilir. Oyuncu, canavarlarla savaşabilir, odalardaki eşyaları toplayabilir ve ilerlemek için stratejiler(canavarın özellikleri sizinkinden fazlaysa görünmeden diğer odaya geçmek vb.) geliştirebilir.

- **Odalar**: Oyuncu odalar arasında hareket edebilir. Her odada bir açıklama, bir veya daha fazla eşya ve bazen bir canavar bulunur.
- **Hareket**: Oyuncu "move [direction]" komutuyla, odalar arasındaki bağlantılara göre yukarı, aşağı, sola veya sağa hareket edebilir.
- **Eşyalar**: Oyuncu, odalarda bulduğu eşyaları envanterine alabilir. Envanterde yer sınırlıdır ve her oda yalnızca belirli eşyalar içerebilir.
- **Savaş**: Oyuncu, bir odada canavar varsa, "attack" komutuyla savaşabilir.Bu savaş sıra tabanlıdır ve oyuncunun vuruşuyla başlar.İki taraftan birinin sağlığı tükenene kadar savaş devam eder.
- **Envanter**: Oyuncu, sahip olduğu eşyaları görebilir.
- **Kayıt**: Oyuncu ilerlemesini kaydedebilir ve daha sonra kaydettiği yerden devam edebilir.

## Kod Yapısı

Kod, farklı işlevlere ve veri yapılarına dayalı olarak yapılandırılmıştır. İşte ana bileşenler:

### Veri Yapıları

- **Room (Oda)**: Odanın açıklamasını, odada bulunan canavarı ve bağlantılı olduğu odaları içerir. Ayrıca, odada bulunan eşyalar da bu yapıda tutulur.
- **Creature (Canavar)**: Canavarlar için bir yapı tanımlanmıştır. Bu yapı, canavarın ismini, sağlığını ve gücünü tutar.
- **Player (Oyuncu)**: Oyuncunun sağlık, güç, envanterini tutan bir yapı vardır.

### Ana Fonksiyonlar

- **look**: Oyuncu, bu komutla bulunduğu odayı ve içeriğini (canavarlar, eşyalar) görebilir.
- **move**: Oyuncu, bu komutla odalar arasında hareket edebilir. Yönler (yukarı, aşağı, sağ, sol) belirlenmiştir.
- **pickup**: Oyuncu, odada bulduğu eşyayı envanterine alabilir.
- **attack**: Oyuncu, odadaki canavara saldırır. Savaş, iki taraftan birinin sağlığı sıfıra düşene kadar devam eder.
- **inventory**: Oyuncu, envanterinde bulunan eşyaları görüntüleyebilir.
- **stats**: Oyuncunun ve odadaki canavarın sağlık ve güç bilgilerini gösterir.
- **save/load**: Oyuncu, ilerlemesini kaydedebilir ve kaydettiği yerden yükleyebilir.
- **controls**: Oyuncuya oyunun tüm komutlarını ve nasıl oynanacağını açıklayan bir fonksiyon.

### Oyun Mantığı

**Keşif ve savaş**: Oyun, odalar arasında hareket etmeyi, eşya toplamayı ve canavarlara karşı savaşı içerir.
**Strateji ve yönetim**: Oyuncunun sağlığı, gücü ve envanteri sınırlıdır, bu yüzden kaynaklarını doğru yönetmelidir.
**İlerleme**: Oyuncu zindanın odalarını keşfederken, zorlukların üstesinden gelerek oyunun sonunda başarılı olmayı hedefler.
Bu oyun mantığı, oyuncunun düşünmesini, stratejik kararlar almasını ve kaynakları verimli kullanmasını gerektirir.

### Kontroller ve Komutlar

- **look**: Mevcut odayı ve içeriğini gösterir.
- **move [direction]**: Belirtilen yönde hareket eder (yukarı, aşağı, sağ, sol).
- **pickup [item]**: Odayızdaki belirli bir eşyayı alır.
- **attack**: Odayınızdaki canavara saldırır.
- **inventory**: Envanterinizi gösterir.
- **stats**: Oyuncunun ve varsa canavarın sağlık ve güç bilgilerini gösterir.
- **save [filename]**: Oyunun kaydedildiği dosyayı belirtir.
- **load [filename]**: Kaydedilen oyunu yükler.
- **exit**: Oyunu sonlandırır.

Bu yapıyı temel alarak oyun, her iki türdeki eylemi (keşif ve savaş) içeriyor ve oyuncunun stratejik düşünmesini teşvik ediyor.
