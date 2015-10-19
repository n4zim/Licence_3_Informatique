import java.io.Serializable;
import java.nio.charset.Charset;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.BitSet;
import java.util.Collection;

public class BloomFilter_Example<E> implements Serializable {
	private static final long serialVersionUID = 1L;
	
	private BitSet bitset;
    private int bitSetSize;
    private double bitsPerElement;
    private int expectedNumberOfFilterElements;
    private int numberOfAddedElements;
    private int k;

    static final Charset charset = Charset.forName("UTF-8");

    static final String hashName = "MD5";
    static final MessageDigest digestFunction;
    static {
        MessageDigest tmp;
        try {
            tmp = java.security.MessageDigest.getInstance(hashName);
        } catch (NoSuchAlgorithmException e) {
            tmp = null;
        }
        digestFunction = tmp;
    }


    public BloomFilter_Example(double c, int n, int k) {
      this.expectedNumberOfFilterElements = n;
      this.k = k;
      this.bitsPerElement = c;
      this.bitSetSize = (int)Math.ceil(c * n);
      numberOfAddedElements = 0;
      this.bitset = new BitSet(bitSetSize);
    }

    public BloomFilter_Example(int bitSetSize, int expectedNumberOElements) {
        this(bitSetSize / (double)expectedNumberOElements,
             expectedNumberOElements,
             (int) Math.round((bitSetSize / (double)expectedNumberOElements) * Math.log(2.0)));
    }

    public BloomFilter_Example(double falsePositiveProbability, int expectedNumberOfElements) {
        this(Math.ceil(-(Math.log(falsePositiveProbability) / Math.log(2))) / Math.log(2), // c = k / ln(2)
             expectedNumberOfElements,
             (int)Math.ceil(-(Math.log(falsePositiveProbability) / Math.log(2)))); // k = ceil(-log_2(false prob.))
    }

    public BloomFilter_Example(int bitSetSize, int expectedNumberOfFilterElements, int actualNumberOfFilterElements, BitSet filterData) {
        this(bitSetSize, expectedNumberOfFilterElements);
        this.bitset = filterData;
        this.numberOfAddedElements = actualNumberOfFilterElements;
    }

    public static int createHash(String val, Charset charset) {
        return createHash(val.getBytes(charset));
    }

    public static int createHash(String val) {
        return createHash(val, charset);
    }

    public static int createHash(byte[] data) {
        return createHashes(data, 1)[0];
    }


    public static int[] createHashes(byte[] data, int hashes) {
        int[] result = new int[hashes];

        int k = 0;
        byte salt = 0;
        while (k < hashes) {
            byte[] digest;
            synchronized (digestFunction) {
                digestFunction.update(salt);
                salt++;
                digest = digestFunction.digest(data);                
            }
        
            for (int i = 0; i < digest.length/4 && k < hashes; i++) {
                int h = 0;
                for (int j = (i*4); j < (i*4)+4; j++) {
                    h <<= 8;
                    h |= ((int) digest[j]) & 0xFF;
                }
                result[k] = h;
                k++;
            }
        }
        return result;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj == null) { return false; }
        if (getClass() != obj.getClass()) { return false; }
        
	@SuppressWarnings("unchecked")
	final BloomFilter_Example<E> other = (BloomFilter_Example<E>) obj;        
        if (this.expectedNumberOfFilterElements != other.expectedNumberOfFilterElements) { return false; }
        if (this.k != other.k) { return false; }
        if (this.bitSetSize != other.bitSetSize) { return false; }
        if (this.bitset != other.bitset && (this.bitset == null || !this.bitset.equals(other.bitset))) { return false; }
        return true;
    }

    @Override
    public int hashCode() {
        int hash = 7;
        hash = 61 * hash + (this.bitset != null ? this.bitset.hashCode() : 0);
        hash = 61 * hash + this.expectedNumberOfFilterElements;
        hash = 61 * hash + this.bitSetSize;
        hash = 61 * hash + this.k;
        return hash;
    }


    public double expectedFalsePositiveProbability() {
        return getFalsePositiveProbability(expectedNumberOfFilterElements);
    }

    public double getFalsePositiveProbability(double numberOfElements) {
        // (1 - e^(-k * n / m)) ^ k
        return Math.pow((1 - Math.exp(-k * (double) numberOfElements
                        / (double) bitSetSize)), k);

    }

    public double getFalsePositiveProbability() {
        return getFalsePositiveProbability(numberOfAddedElements);
    }


    public int getK() {
        return k;
    }

    public void clear() {
        bitset.clear();
        numberOfAddedElements = 0;
    }

    public void add(E element) {
       add(element.toString().getBytes(charset));
    }

    public void add(byte[] bytes) {
       int[] hashes = createHashes(bytes, k);
       for (int hash : hashes)
           bitset.set(Math.abs(hash % bitSetSize), true);
       numberOfAddedElements ++;
    }

    public void addAll(Collection<? extends E> c) {
        for (E element : c)
            add(element);
    }
        
    public boolean contains(E element) {
        return contains(element.toString().getBytes(charset));
    }

    public boolean contains(byte[] bytes) {
        int[] hashes = createHashes(bytes, k);
        for (int hash : hashes) {
            if (!bitset.get(Math.abs(hash % bitSetSize))) {
                return false;
            }
        }
        return true;
    }

    public boolean containsAll(Collection<? extends E> c) {
        for (E element : c)
            if (!contains(element))
                return false;
        return true;
    }

    public boolean getBit(int bit) {
        return bitset.get(bit);
    }

    public void setBit(int bit, boolean value) {
        bitset.set(bit, value);
    }

    public BitSet getBitSet() {
        return bitset;
    }

    public int size() {
        return this.bitSetSize;
    }

    public int count() {
        return this.numberOfAddedElements;
    }

    public int getExpectedNumberOfElements() {
        return expectedNumberOfFilterElements;
    }

    public double getExpectedBitsPerElement() {
        return this.bitsPerElement;
    }

    public double getBitsPerElement() {
        return this.bitSetSize / (double)numberOfAddedElements;
    }
}