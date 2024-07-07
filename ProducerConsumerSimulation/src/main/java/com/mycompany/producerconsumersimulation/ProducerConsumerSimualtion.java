package main.java.com.mycompany.producerconsumersimulation;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Random;
import java.util.concurrent.*;
import javax.swing.*;

class SimulazioneProduttoreConsumatore extends JFrame {

    private static final SimpleDateFormat formatoData = new SimpleDateFormat("HH:mm:ss");

    private final JTextArea logTextArea;
    private final JTextField campoMaxDifficolta;
    private final JTextField campoDimensioneBuffer;
    private final JTextField campoNumProduttori;
    private final JTextField campoNumConsumatori;
    private final JTextField campoIntervalloProduzione;

    private Buffer buffer;
    private ExecutorService esecutoreProduttori;
    private ExecutorService esecutoreConsumatori;

    public SimulazioneProduttoreConsumatore() {
        setTitle("Simulazione Produttore Consumatore");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(600, 400);

        logTextArea = new JTextArea();
        logTextArea.setEditable(false);
        JScrollPane scrollPane = new JScrollPane(logTextArea);

        JPanel pannelloImpostazioni = new JPanel();
        pannelloImpostazioni.setLayout(new GridLayout(5, 2));

        pannelloImpostazioni.add(new JLabel("Difficoltà Massima:"));
        campoMaxDifficolta = new JTextField();
        pannelloImpostazioni.add(campoMaxDifficolta);

        pannelloImpostazioni.add(new JLabel("Dimensione Buffer:"));
        campoDimensioneBuffer = new JTextField();
        pannelloImpostazioni.add(campoDimensioneBuffer);

        pannelloImpostazioni.add(new JLabel("Numero di Produttori:"));
        campoNumProduttori = new JTextField();
        pannelloImpostazioni.add(campoNumProduttori);

        pannelloImpostazioni.add(new JLabel("Numero di Consumatori:"));
        campoNumConsumatori = new JTextField();
        pannelloImpostazioni.add(campoNumConsumatori);

        pannelloImpostazioni.add(new JLabel("Intervallo di Produzione (ms):"));
        campoIntervalloProduzione = new JTextField();
        pannelloImpostazioni.add(campoIntervalloProduzione);

        JButton bottoneAvvia = new JButton("Avvia Simulazione");
        bottoneAvvia.addActionListener((ActionEvent e) -> {
            avviaSimulazione();
        });

        JPanel pannelloBottoni = new JPanel();
        pannelloBottoni.add(bottoneAvvia);

        add(pannelloImpostazioni, BorderLayout.NORTH);
        add(scrollPane, BorderLayout.CENTER);
        add(pannelloBottoni, BorderLayout.SOUTH);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            new SimulazioneProduttoreConsumatore().setVisible(true);
        });
    }

    private void avviaSimulazione() {
        SwingUtilities.invokeLater(() -> {
            try {
                int maxDifficolta = Integer.parseInt(campoMaxDifficolta.getText());
                int dimensioneBuffer = Integer.parseInt(campoDimensioneBuffer.getText());
                int numProduttori = Integer.parseInt(campoNumProduttori.getText());
                int numConsumatori = Integer.parseInt(campoNumConsumatori.getText());
                int intervalloProduzione = Integer.parseInt(campoIntervalloProduzione.getText());

                if (maxDifficolta <= 0 || dimensioneBuffer <= 0 || numProduttori <= 0 || numConsumatori <= 0 || intervalloProduzione <= 0) {
                    log("Errore: Assicurarsi che tutti i campi siano numeri positivi.");
                    return;
                }

                buffer = new Buffer(dimensioneBuffer);
                esecutoreProduttori = Executors.newFixedThreadPool(numProduttori);
                esecutoreConsumatori = Executors.newFixedThreadPool(numConsumatori);

                // Avvia produttori
                for (int i = 0; i < numProduttori; i++) {
                    esecutoreProduttori.execute(new Produttore(i + 1, maxDifficolta, intervalloProduzione));
                }

                // Avvia consumatori
                for (int i = 0; i < numConsumatori; i++) {
                    esecutoreConsumatori.execute(new Consumatore(i + 1));
                }
            } catch (NumberFormatException e) {
                log("Errore: Assicurarsi che tutti i campi siano numeri validi.");
            } catch (Exception ex) {
                log("Errore: " + ex.getMessage());
            }
        });
    }

    private void log(String messaggio) {
        SwingUtilities.invokeLater(() -> {
            String timestamp = formatoData.format(new Date());
            String messaggioLog = timestamp + " - " + messaggio + "\n";
            logTextArea.append(messaggioLog);
            logTextArea.setCaretPosition(logTextArea.getDocument().getLength()); // Scorrimento automatico fino in fondo
            logTextArea.repaint(); // Aggiornamento GUI
        });
    }

    private class Oggetto {
        private final String nome;
        private final int difficolta;

        public Oggetto(String nome, int difficolta) {
            this.nome = nome;
            this.difficolta = difficolta;
        }

        public String getNome() {
            return nome;
        }

        public int getDifficolta() {
            return difficolta;
        }

        @Override
        public String toString() {
            return "Oggetto{" +
                    "nome='" + nome + '\'' +
                    ", difficolta=" + difficolta +
                    '}';
        }
    }

    private class Produttore implements Runnable {
        private final int id;
        private final Random random = new Random();
        private final int maxDifficolta;
        private final int intervalloProduzione;

        public Produttore(int id, int maxDifficolta, int intervalloProduzione) {
            this.id = id;
            this.maxDifficolta = maxDifficolta;
            this.intervalloProduzione = intervalloProduzione;
        }

        @Override
        public void run() {
            try {
                while (!Thread.currentThread().isInterrupted()) {
                    int difficolta = random.nextInt(maxDifficolta) + 1;
                    String nome;
                    if (random.nextBoolean()) {
                        nome = "Pasta_" + System.currentTimeMillis();
                    } else {
                        nome = "Pizza_" + System.currentTimeMillis();
                    }
                    Oggetto oggetto = new Oggetto(nome, difficolta);
                    produci(oggetto);
                    log("Produttore " + id + " - Ha prodotto Oggetto " + nome + " - Difficoltà " + difficolta);
                    Thread.sleep(intervalloProduzione);
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
    }

    private class Consumatore implements Runnable {
        private final int id;

        public Consumatore(int id) {
            this.id = id;
        }

        @Override
        public void run() {
            try {
                while (!Thread.currentThread().isInterrupted()) {
                    Oggetto oggetto = consuma();
                    if (oggetto != null) {
                        log("Consumatore " + id + " - Iniziato consumo - " + oggetto.getNome());
                        Thread.sleep(oggetto.getDifficolta() * 1000);
                        log("Consumatore " + id + " - Finito consumo - " + oggetto.getNome());
                    }
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
    }

    private void produci(Oggetto oggetto) throws InterruptedException {
        synchronized (buffer) {
            while (buffer.isFull()) {
                log("Produttore - Buffer pieno, in attesa...");
                buffer.wait(5000);
            }
            buffer.put(oggetto);
            buffer.notifyAll();
        }
    }

    private Oggetto consuma() throws InterruptedException {
        synchronized (buffer) {
            while (buffer.isEmpty()) {
                log("Consumatore - Buffer vuoto, in attesa...");
                buffer.wait(5000);
            }
            Oggetto oggetto = buffer.take();
            buffer.notifyAll();
            return oggetto;
        }
    }

    public class Buffer {
        private final ArrayList<Oggetto> buffer;
        private final int maxSize;

        public Buffer(int maxSize) {
            this.maxSize = maxSize;
            this.buffer = new ArrayList<>();
        }

        public synchronized void put(Oggetto oggetto) throws InterruptedException {
            while (buffer.size() >= maxSize) {
                wait(); // Attende finché il buffer non diventa disponibile
            }
            buffer.add(oggetto);
            notifyAll(); // Notifica i thread in attesa su questo oggetto
        }

        public synchronized Oggetto take() throws InterruptedException {
            while (buffer.isEmpty()) {
                wait(); // Attende finché il buffer non contiene elementi
            }
            Oggetto oggetto = buffer.remove(0);
            notifyAll(); // Notifica i thread in attesa su questo oggetto
            return oggetto;
        }

        public boolean isFull() {
            return buffer.size() >= maxSize;
        }

        public boolean isEmpty() {
            return buffer.isEmpty();
        }
    }
}
