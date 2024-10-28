package com.jiaming.mycompsciproblems;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.text.TextUtils;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.util.BitSet;
import java.util.HashMap;
import java.util.Map;

public class SimpleMathActivity extends AppCompatActivity implements View.OnClickListener{
    private EditText editValue;
    private Button btnCalcFib;
    // fib data structure
    static Map<Integer, Integer> mapFibMemory = new HashMap<>();

    // compress components
    private EditText editCommpressValue;
    private Button btnCommpressTest;
    // compress data structure
    private BitSet bitSetCompressGene;

    // encrypt components
    private EditText editEncryptValue;
    private Button btnEncryptTest;

    // Pi calculate components
    private Button btnPiTest;
    private Button btnHanoiTest;

    private TextView textTestResult;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_simple_math);

        editValue = (EditText)findViewById(R.id.item_value);
        btnCalcFib = (Button)findViewById(R.id.calc_fib);
        btnCalcFib.setOnClickListener(this);

        editCommpressValue = (EditText)findViewById(R.id.compress_value);
        btnCommpressTest = (Button)findViewById(R.id.compress_test);
        btnCommpressTest.setOnClickListener(this);

        editEncryptValue = (EditText)findViewById(R.id.encrypt_value);
        btnEncryptTest = (Button)findViewById(R.id.encrypt_test);
        btnEncryptTest.setOnClickListener(this);

        btnPiTest = (Button)findViewById(R.id.pi_test);
        btnPiTest.setOnClickListener(this);
        btnHanoiTest = (Button)findViewById(R.id.hanoi_test);
        btnHanoiTest.setOnClickListener(this);

        textTestResult = (TextView)findViewById(R.id.test_result);

        // This creates a map with 0->0 and 1->1
        // Which represent our base cases
        mapFibMemory.put(0, 0);
        mapFibMemory.put(1, 1);

        editValue.setText("40");
        editCommpressValue.setText("TAGGGATTAACCGTTATATATATATAGCCATGGATCGATTATATAGGGATTAACCGTTATATATATATAGCCATGGATCGATTATA");
        editEncryptValue.setText("One Time Pad!");
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.calc_fib:
                if (TextUtils.isEmpty(editValue.getText().toString())){
                    Toast.makeText(SimpleMathActivity.this, "请输入数值!", Toast.LENGTH_SHORT).show();
                    return;
                }
                Integer iValue = Integer.valueOf(editValue.getText().toString());
                if(iValue > 0)
                {
                    // Integer result = calc_fib(iValue);
                    Integer result = no_recusive_calc_fib(iValue);
                    textTestResult.setText(result.toString());
                }
                break;
            case R.id.compress_test:
                if (TextUtils.isEmpty(editCommpressValue.getText().toString())){
                    Toast.makeText(SimpleMathActivity.this, "请输入数值!", Toast.LENGTH_SHORT).show();
                    return;
                }
                int iLen = compress(editCommpressValue.getText().toString());
                String strDecommpressResult = decompress(iLen);
                textTestResult.setText(strDecommpressResult);
                break;
            case R.id.encrypt_test:
                if (TextUtils.isEmpty(editEncryptValue.getText().toString())){
                    Toast.makeText(SimpleMathActivity.this, "请输入数值!", Toast.LENGTH_SHORT).show();
                    return;
                }
                UnbreakableEncryption objEncryption = new UnbreakableEncryption();
                KeyPair keyPair = objEncryption.encrypt(editEncryptValue.getText().toString());
                String strEncryptResult = objEncryption.decrypt(keyPair);
                textTestResult.setText(strEncryptResult);
                break;
            case R.id.pi_test:
                double dPiResult = calculatePi(1000000);
                textTestResult.setText(String.valueOf(dPiResult));
                break;
            case R.id.hanoi_test:
                Hanoi hanoi = new Hanoi(3);
                hanoi.solve();
                textTestResult.setText("A = " + hanoi.towerA + "B = " + hanoi.towerB + "C = " + hanoi.towerC);
                break;
        }
    }

    @Override
    public void onContextMenuClosed(Menu menu) {
        super.onContextMenuClosed(menu);
    }

    private static int calc_fib(int n) {
        // 如果不存在，就生成一个放进去。
        if(!mapFibMemory.containsKey(n)){
            mapFibMemory.put(n, calc_fib(n - 1) + calc_fib(n - 2));
        }
        // 如果存在，就返回计算好的。否则就返回刚刚放进去的。
        return mapFibMemory.get(n);
    }

    private static int no_recusive_calc_fib(int n) {
        int last =0, next = 1; // fib(0), fib(1)
        for (int i = 0; i < n; i++){
            // 使用临时变量oldLast在交换过程中进行过渡。
            int oldLst = last;
            // 要点是last被设置为next的前一个值，next被设置为last的前一个值加上next的前一个值。
            last = next;
            next = oldLst + next;
        }
        return last;
    }

    // CompressedGene
    private int compress(String gene) {
        int geneLength = gene.length();
        bitSetCompressGene = new BitSet(geneLength * 2);

        final String strUppercaseGene = gene.toUpperCase();
        for (int i =0; i < geneLength; i++){
            final int firstLocation = 2 * i;
            final int secondLocation = 2 * i + 1;
            switch (strUppercaseGene.charAt(i)) {
                case 'A':
                    bitSetCompressGene.set(firstLocation, false);
                    bitSetCompressGene.set(secondLocation, false);
                    break;
                case 'C':
                    bitSetCompressGene.set(firstLocation, false);
                    bitSetCompressGene.set(secondLocation, true);
                    break;
                case 'G':
                    bitSetCompressGene.set(firstLocation, true);
                    bitSetCompressGene.set(secondLocation, false);
                    break;
                case 'T':
                    bitSetCompressGene.set(firstLocation, true);
                    bitSetCompressGene.set(secondLocation, true);
                    break;
                default:
                    throw new IllegalArgumentException("The provided gene String contains characters other than ACGT");
            }
        }
        return geneLength;
    }

    public String decompress(int geneLength) {
        if (bitSetCompressGene == null) {
            return "";
        }
        else {
            StringBuffer builder = new StringBuffer(geneLength);
            for (int i =0; i < geneLength * 2; i++) {
                final int firstBit = (bitSetCompressGene.get(i) ? 1 : 0);
                final int secondBit = (bitSetCompressGene.get(i + 1) ? 1 : 0);
                final int lastBits = firstBit << 1 | secondBit;
                switch (lastBits) {
                    case 0b00:
                        builder.append('A');
                        break;
                    case 0b01:
                        builder.append('C');
                        break;
                    case 0b10:
                        builder.append('G');
                        break;
                    case 0b11:
                        builder.append('T');
                        break;
                }
            }
            return builder.toString();
        }
    }

    public static double calculatePi(int nTerms) {
        final double numberator = 4.0;
        double denominator = 1.0;
        double operation = 1.0;
        double pi = 0.0;
        for (int i = 0; i < nTerms; i++) {
            pi += operation * (numberator / denominator);
            denominator += 2.0;
            operation *= -1.0;
        }
        return pi;
    }
}
