// @category BLUR

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSpace;
import ghidra.program.model.listing.Function;

import java.io.BufferedWriter;
import java.io.File;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.List;

public class ExportTargetDecomp extends GhidraScript {

    private static final Target[] TARGETS = new Target[] {
        new Target("main_kernel", 0x3766L),
        new Target("variance_dispatch", 0x4080L),
        new Target("variance_case_0", 0x4090L),
        new Target("variance_case_1", 0x4218L),
        new Target("variance_case_2", 0x4238L),
        new Target("variance_case_3", 0x427eL),
        new Target("variance_case_4", 0x42b8L),
        new Target("hot_wrapper", 0x45caL),
        new Target("helper_called_from_wrapper", 0xa0aeL),
        new Target("helper_get_sample_rate", 0xcbceL),
        new Target("helper_init_fft_support", 0xd2a7L),
        new Target("helper_window_setup", 0xd30aL),
        new Target("helper_hop_insert", 0xd3a7L),
        new Target("helper_pre_fft_window", 0xd433L),
        new Target("helper_overlap_add", 0xde6fL),
        new Target("helper_phase_rotate", 0xdf77L),
    };

    @Override
    protected void run() throws Exception {
        String[] args = getScriptArgs();
        File outputDir = args.length > 0
            ? new File(args[0])
            : new File(System.getProperty("user.home"), "ghidra-exports");

        if (!outputDir.isDirectory() && !outputDir.mkdirs()) {
            throw new IllegalStateException("Failed to create output directory: " + outputDir);
        }

        String baseName = sanitize(currentProgram.getName()) + "__" +
            sanitize(String.valueOf(currentProgram.getLanguageID()));

        DecompInterface decompiler = new DecompInterface();
        decompiler.openProgram(currentProgram);

        List<Record> records = new ArrayList<>();
        try {
            for (Target target : TARGETS) {
                records.add(processTarget(decompiler, outputDir, baseName, target));
            }
        }
        finally {
            decompiler.dispose();
        }

        File summaryFile = new File(outputDir, baseName + "_target_decomp.json");
        try (BufferedWriter writer = Files.newBufferedWriter(summaryFile.toPath(), StandardCharsets.UTF_8)) {
            writer.write(toJson(records));
        }

        println("Wrote target decompilation summary to " + summaryFile.getAbsolutePath());
    }

    private Record processTarget(DecompInterface decompiler, File outputDir, String baseName, Target target)
        throws Exception {

        LocatedFunction located = findFunction(target.offset);
        Record record = new Record();
        record.label = target.label;
        record.requestedOffset = String.format("0x%x", target.offset);

        if (located == null) {
            record.status = "not_found";
            return record;
        }

        record.matchedAddress = located.address.toString();
        record.functionName = located.function.getName(true);

        DecompileResults results = decompiler.decompileFunction(located.function, 60, monitor);
        String decompiled;
        if (results != null && results.decompileCompleted()) {
            record.status = "decompiled";
            decompiled = results.getDecompiledFunction().getC();
        }
        else {
            record.status = "failed";
            String error = results != null ? results.getErrorMessage() : "unknown decompiler failure";
            decompiled = "/* Decompilation failed: " + error + " */\n";
        }

        File outputFile = new File(outputDir,
            baseName + "__" + target.label + "__" + String.format("0x%x", target.offset) + ".c");

        StringBuilder builder = new StringBuilder();
        builder.append("// program: ").append(currentProgram.getName()).append('\n');
        builder.append("// language: ").append(currentProgram.getLanguageID()).append('\n');
        builder.append("// requested_offset: ").append(record.requestedOffset).append('\n');
        builder.append("// matched_address: ").append(record.matchedAddress).append('\n');
        builder.append("// function_name: ").append(record.functionName).append("\n\n");
        builder.append(decompiled);

        try (BufferedWriter writer = Files.newBufferedWriter(outputFile.toPath(), StandardCharsets.UTF_8)) {
            writer.write(builder.toString());
        }

        record.outputFile = outputFile.getAbsolutePath();
        return record;
    }

    private LocatedFunction findFunction(long offset) {
        for (Address address : candidateAddresses(offset)) {
            Function function = getFunctionAt(address);
            if (function == null) {
                function = getFunctionContaining(address);
            }
            if (function != null) {
                return new LocatedFunction(function, address);
            }
        }
        return null;
    }

    private List<Address> candidateAddresses(long offset) {
        AddressSpace space = currentProgram.getAddressFactory().getDefaultAddressSpace();
        long imageBase = currentProgram.getImageBase().getOffset();
        List<Address> addresses = new ArrayList<>();
        addAddress(space, addresses, offset);
        addAddress(space, addresses, imageBase + offset);
        return addresses;
    }

    private void addAddress(AddressSpace space, List<Address> addresses, long value) {
        try {
            Address address = space.getAddress(value);
            if (!addresses.contains(address)) {
                addresses.add(address);
            }
        }
        catch (Exception ignored) {
        }
    }

    private String toJson(List<Record> records) {
        StringBuilder json = new StringBuilder();
        json.append("{\n");
        json.append("  \"program_name\": \"").append(jsonEscape(currentProgram.getName())).append("\",\n");
        json.append("  \"language_id\": \"").append(jsonEscape(String.valueOf(currentProgram.getLanguageID()))).append("\",\n");
        json.append("  \"targets\": [\n");

        for (int index = 0; index < records.size(); index++) {
            Record record = records.get(index);
            json.append("    {\n");
            json.append("      \"label\": \"").append(jsonEscape(record.label)).append("\",\n");
            json.append("      \"requested_offset\": \"").append(jsonEscape(record.requestedOffset)).append("\",\n");
            json.append("      \"matched_address\": ")
                .append(record.matchedAddress != null ? "\"" + jsonEscape(record.matchedAddress) + "\"" : "null")
                .append(",\n");
            json.append("      \"function_name\": ")
                .append(record.functionName != null ? "\"" + jsonEscape(record.functionName) + "\"" : "null")
                .append(",\n");
            json.append("      \"status\": \"").append(jsonEscape(record.status)).append("\",\n");
            json.append("      \"output_file\": ")
                .append(record.outputFile != null ? "\"" + jsonEscape(record.outputFile) + "\"" : "null")
                .append("\n");
            json.append("    }");
            if (index + 1 < records.size()) {
                json.append(',');
            }
            json.append("\n");
        }

        json.append("  ]\n");
        json.append("}\n");
        return json.toString();
    }

    private String sanitize(String value) {
        return value.replaceAll("[^A-Za-z0-9._-]", "_");
    }

    private String jsonEscape(String value) {
        return value
            .replace("\\", "\\\\")
            .replace("\"", "\\\"")
            .replace("\r", "\\r")
            .replace("\n", "\\n")
            .replace("\t", "\\t");
    }

    private static final class Target {
        private final String label;
        private final long offset;

        private Target(String label, long offset) {
            this.label = label;
            this.offset = offset;
        }
    }

    private static final class LocatedFunction {
        private final Function function;
        private final Address address;

        private LocatedFunction(Function function, Address address) {
            this.function = function;
            this.address = address;
        }
    }

    private static final class Record {
        private String label;
        private String requestedOffset;
        private String matchedAddress;
        private String functionName;
        private String status;
        private String outputFile;
    }
}